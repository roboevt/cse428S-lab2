// HoldEmGame.cpp   Eric Todd   e.todd@wustl.edu    Definitions for HoldEmGame class. Constructor,
// dealing, and turn handling.

#include "HoldEmGame.h"

#include <algorithm>
#include <array>
#include <optional>

HoldEmGame::HoldEmGame(int numPlayers, const char** playerNames)
    : Game(numPlayers, playerNames), state(HoldEmState::preflop) {
    hands.resize(numPlayers);
}

void HoldEmGame::deal() {
    switch (state) {
        case HoldEmState::preflop:
            // Distribute cards to each player
            for (int i = 0; i < HAND_SIZE; i++) {
                for (size_t j = 0; j < hands.size(); j++) {
                    if (!deck.is_empty()) deck >> hands[j];
                }
            }
            state = HoldEmState::flop;
            break;
        case HoldEmState::flop:
            // Add flop cards to common cards
            for (int i = 0; i < FLOP_COMMON_SIZE; i++) {
                if (!deck.is_empty()) deck >> commonCards;
            }
            state = HoldEmState::turn;
            break;
        case HoldEmState::turn:
            // Add turn card to common cards
            if (!deck.is_empty()) deck >> commonCards;
            state = HoldEmState::river;
            break;
        case HoldEmState::river:
            // Add river card to common cards
            if (!deck.is_empty()) deck >> commonCards;
            state = HoldEmState::undefined;
            break;
        default:
            break;
    }
}

int HoldEmGame::play() {
    // Play one round and then ask the user if they want to play again
    do {
        state = HoldEmState::preflop;
        deck.shuffle();

        while (state != HoldEmState::undefined) {  // Move through each state
            deal();
            printState();

            // Evaluate hands after the flop
            if (state == HoldEmState::turn) evaluate();
        }

        collectCards();
    } while (!promptUserEnd());

    return 0;
}

void HoldEmGame::evaluate() {
    // all Players
    std::vector<HoldEmGame::Player> playerHands;
    for (size_t i = 0; i < hands.size(); i++) {
        CardSet<HoldEmRank, Suit> hand = hands[i];  // Copy hand
        auto commonCopy = commonCards;
        while (!commonCopy.is_empty()) commonCopy >> hand;  // Add all common cards to hand

        // Evaluate current hand
        HoldEmHandRank handRank = holdem_hand_evaluation(hand);

        // Create player object
        Player player(hand, players[i], handRank);
        playerHands.push_back(player);
    }

    // Sort player hands by handrank
    std::sort(playerHands.begin(), playerHands.end());

    // Display results
    for (int i = playerHands.size() - 1; i >= 0; i--) {
        std::cout << playerHands[i].name << ": "
                  << "\tCards: ";
        playerHands[i].hand.print(std::cout, FINAL_HAND_SIZE + HAND_SIZE + 1);
        std::cout << "\t Rank: " << playerHands[i].handRank << std::endl;
    }
}

void HoldEmGame::printState() const {
    switch (state) {
        // Print based on previous state because print will be called after state is updated in
        // deal()
        case HoldEmState::flop:  // print hands
            for (size_t i = 0; i < hands.size(); i++) {
                std::cout << players[i] << ": ";
                hands[i].print(std::cout, HAND_SIZE);
            }
            break;

        // print common cards and current state
        case HoldEmState::turn:
            std::cout << "BOARD(flop): ";
            commonCards.print(std::cout, FLOP_COMMON_SIZE);
            break;
        case HoldEmState::river:
            std::cout << "BOARD(turn): ";
            commonCards.print(std::cout, FLOP_COMMON_SIZE + 1);
            break;
        case HoldEmState::undefined:
            std::cout << "BOARD(river): ";
            commonCards.print(std::cout, FLOP_COMMON_SIZE + 2);
            break;
        default:
            break;
    }
}

void HoldEmGame::collectCards() {
    // Move all cards from hands to deck
    for (auto& hand : hands) {
        while (!hand.is_empty()) {
            hand >> deck;
        }
    }
    // Move all cards from commonCards to deck
    while (!commonCards.is_empty()) {
        commonCards >> deck;
    }
}

// Anonymous namespace for helper functions, keeps them from being visible outside this file (like
// static but apparently better style)
namespace {

// Aliases to simplify function signatures
using Pair = std::array<Card<HoldEmRank, Suit>, 2>;
using Hand = std::vector<Card<HoldEmRank, Suit>>;

/// @brief Check a hand for a straight flush
/// @param hand The hand to check sorted by rank
/// @return if all five cards are of the same suit and their ranks are consecutive (with the
/// special rule that A 2 3 4 5 of the same suit and 10 J Q K A of the same suit are the lowest and
/// highest valid straight flushes respectively, but that any hand with cards ranked K A 2 is not
/// considered a straight flush)
bool checkStraightFlush(const Hand& hand) {
    Suit suit = hand[0].suit;
    int rank = static_cast<int>(hand[0].rank);

    bool isStraightFlush = true;
    for (size_t i = 1; i < hand.size(); i++) {
        if (hand[i].suit != suit || static_cast<int>(hand[i].rank) != rank + 1) {
            isStraightFlush = false;
            break;
        }
        rank++;
    }

    // Check for ace low straight
    if (!isStraightFlush && hand[0].rank == HoldEmRank::ace) {
        isStraightFlush = true;
        rank = static_cast<int>(HoldEmRank::two) - 1;
        for (size_t i = 1; i < hand.size(); i++) {
            if (hand[i].suit != suit || static_cast<int>(hand[i].rank) != rank + 1) {
                isStraightFlush = false;
                break;
            }
            rank++;
        }
    }

    return isStraightFlush;
}

/// @brief Check a hand for four of a kind
/// @param hand The hand to check sorted by rank
/// @return if four cards have the same rank
bool checkFourOfAKind(const Hand& hand, HoldEmRank& fourRank) {
    bool isFourOfAKind = false;

    // Two cases, x x x x y or x y y y y

    if (hand[0].rank == hand[3].rank || hand[1].rank == hand[4].rank) {
        isFourOfAKind = true;
        fourRank = hand[2].rank;  // Second card is always in the four of a kind
    }

    return isFourOfAKind;
}

/// @brief Overload of checkFourOfAKind with no output parameter
/// @param hand Sorted hand to check for four of a kind
/// @return if four cards have the same rank
bool checkFourOfAKind(const Hand& hand) {
    HoldEmRank unused;
    return checkFourOfAKind(hand, unused);
}

/// @brief Check a hand for a full house
/// @param hand The hand to check sorted by rank
/// @return if three cards have the same rank and the other two cards have the same rank
bool checkFullHouse(const Hand& hand) {
    bool isFullHouse = false;

    // Two cases, x x x y y or x x y y y

    if (hand[0].rank == hand[2].rank && hand[3].rank == hand[4].rank) {
        isFullHouse = true;
    } else if (hand[0].rank == hand[1].rank && hand[2].rank == hand[4].rank) {
        isFullHouse = true;
    }
    return isFullHouse;
}

/// @brief Check a hand for a flush
/// @param hand The hand to check sorted by rank
/// @return  if all five cards are of the same suit
bool checkFlush(const Hand& hand) {
    Suit suit = hand[0].suit;
    bool isFlush = true;
    for (size_t i = 1; i < hand.size(); i++) {
        if (hand[i].suit != suit) {
            isFlush = false;
            break;
        }
    }
    return isFlush;
}

/// @brief Check a hand for a straight
/// @param hand The hand to check sorted by rank
/// @return if the ranks of the cards are consecutive (with the special rule that A 2 3 4 5 and 10 J
/// Q K A are the lowest and highest valid straights respectively, but any hand with cards ranked K
/// A 2 is not considered a straight)
bool checkStraight(const Hand& hand, Card<HoldEmRank, Suit>& highestCard) {
    int rank = static_cast<int>(hand[0].rank);
    bool isStraight = true;
    for (size_t i = 1; i < hand.size(); i++) {
        if (static_cast<int>(hand[i].rank) != rank + 1) {
            isStraight = false;
            break;
        }
        rank++;
    }
    if (isStraight) {
        highestCard = hand[hand.size() - 1];
        return true;
    }

    // Check for ace low straight
    if (hand[0].rank == HoldEmRank::ace) {
        isStraight = true;
        rank = static_cast<int>(HoldEmRank::two) - 1;
        for (size_t i = 1; i < hand.size(); i++) {
            if (static_cast<int>(hand[i].rank) != rank + 1) {
                isStraight = false;
                break;
            }
            rank++;
        }
    }

    highestCard = hand[hand.size() - 1];
    return isStraight;
}

/// @brief Overload of checkStraight with no output parameter
/// @param hand Sorted hand to check for a straight
/// @return if the ranks of the cards are consecutive (with the special rule that A 2 3 4 5 and 10 J
/// Q K A are the lowest and highest valid straights respectively, but any hand with cards ranked K
/// A 2 is not considered a straight)
bool checkStraight(const Hand& hand) {
    Card<HoldEmRank, Suit> unused;
    return checkStraight(hand, unused);
}

/// @brief Check a hand for three of a kind
/// @param hand The hand to check sorted by rank
/// @param threeOfAKind Output parameter to store the three cards that have the same rank
/// @return if three cards have the same rank and the other two cards are of different ranks than
/// any other cards in the hand
bool checkThreeOfAKind(const Hand& hand, std::vector<Card<HoldEmRank, Suit>>& threeOfAKind) {
    if (checkFourOfAKind(hand)) return false;  // The other two cards must be different ranks

    bool isThreeOfAKind = false;

    // Three cases, x x x y z, x y y y z, x y z z z
    // Because the cards are sorted, we can just check the first and last of the three cards

    if (hand[0].rank == hand[2].rank) {
        isThreeOfAKind = true;
        threeOfAKind = {hand[0], hand[1], hand[2]};
    } else if (hand[1].rank == hand[3].rank) {
        isThreeOfAKind = true;
        threeOfAKind = {hand[1], hand[2], hand[3]};
    } else if (hand[2].rank == hand[4].rank) {
        isThreeOfAKind = true;
        threeOfAKind = {hand[2], hand[3], hand[4]};
    }

    return isThreeOfAKind;
}

/// @brief Overload of checkThreeOfAKind with no output parameter
/// @param hand Sorted hand to check for three of a kind
/// @return if three cards have the same rank and the other two cards are of different ranks than
/// any other cards in the hand
bool checkThreeOfAKind(const Hand& hand) {
    std::vector<Card<HoldEmRank, Suit>> unused;
    return checkThreeOfAKind(hand, unused);
}

/// @brief Check a hand for two pair
/// @param hand The hand to check sorted by rank
/// @param pair1 Output parameter to store the first pair
/// @param pair2 Output parameter to store the second pair
/// @return if two cards have the same rank and two other cards share a different rank and the fifth
/// card is of yet another rank card is of yet another rank
bool checkTwoPair(const Hand& hand, Pair& pair1, Pair& pair2) {
    // Both pairs must be different and the other card must be different
    if (checkFourOfAKind(hand) || checkFullHouse(hand)) return false;

    bool isTwoPair = false;

    // Three cases, x x y y z, x y y z z, x x y z z

    if (hand[0].rank == hand[1].rank && hand[2].rank == hand[3].rank) {
        isTwoPair = true;
        pair1 = {hand[0], hand[1]};
        pair2 = {hand[2], hand[3]};
    } else if (hand[1].rank == hand[2].rank && hand[3].rank == hand[4].rank) {
        isTwoPair = true;
        pair1 = {hand[1], hand[2]};
        pair2 = {hand[3], hand[4]};
    } else if (hand[0].rank == hand[1].rank && hand[3].rank == hand[4].rank) {
        isTwoPair = true;
        pair1 = {hand[0], hand[1]};
        pair2 = {hand[3], hand[4]};
    }

    return isTwoPair;
}

/// @brief Overload of checkTwoPair with no output parameters
/// @param hand Sorted hand to check for two pair
/// @return if two cards have the same rank and two other cards share a different rank and the fifth
/// card is of yet another rank
bool checkTwoPair(const Hand& hand) {
    Pair unused1, unused2;
    return checkTwoPair(hand, unused1, unused2);
}

/// @brief Check a hand for a pair
/// @param hand The hand to check sorted by rank
/// @param pair Output parameter to store the pair
/// @return if only two cards in the hand have the same rank
bool checkPair(const Hand& hand, Pair& pair) {
    // The other three cards must be different ranks
    if (checkFourOfAKind(hand) || checkFullHouse(hand) || checkThreeOfAKind(hand) ||
        checkTwoPair(hand))
        return false;

    bool isPair = false;

    // Four cases, x x y z w, x y y z w, x y z z w, x y z w w

    if (hand[0].rank == hand[1].rank) {
        isPair = true;
        pair = {hand[0], hand[1]};
    } else if (hand[1].rank == hand[2].rank) {
        isPair = true;
        pair = {hand[1], hand[2]};
    } else if (hand[2].rank == hand[3].rank) {
        isPair = true;
        pair = {hand[2], hand[3]};
    } else if (hand[3].rank == hand[4].rank) {
        isPair = true;
        pair = {hand[3], hand[4]};
    }

    return isPair;
}

/// @brief Overload of checkPair with no output parameter
/// @param hand Sorted hand to check for a pair
/// @return if only two cards in the hand have the same rank
bool checkPair(const Hand& hand) {
    Pair unused;
    return checkPair(hand, unused);
}

// This would normally be a function but functions can not have auto parameters until C++20 and we
// must use C++17 in this class.

/// @brief Find the maximum card in a hand that is not in another hand
/// @param hand Hand to search
/// @param excluded Hand to exclude
/// @return Maximum card accoring to compareRank() that is in hand but not in excluded
constexpr auto findMaxCardNotExcluded = [](const Hand& hand,
                                           const auto& excluded) -> Card<HoldEmRank, Suit> {
    Hand diff;

    std::set_difference(hand.begin(), hand.end(), excluded.begin(), excluded.end(),
                        std::inserter(diff, diff.begin()));

    return *std::max_element(diff.begin(), diff.end());
};

/// @brief Check if a hand with a pair is less than another hand with a pair
/// @param lHand Sorted hand with a pair
/// @param rHand Sorted hand with a pair
/// @return if the rank of the paired card of the first object is less than the rank of the paired
/// card of second object, or if those are the same, if the highest-ranked non-paired card of the
/// first object is less than that of the second object, or if those are the same if the
/// second-highest-ranked non-paired card of the first object is less than that of the second
/// object, or if those are the same if the third-highest-ranked non-paired card of the first object
/// is less than that of the second object;
bool pairLessThan(const Hand& lHand, const Hand& rHand) {
    Pair lPair, rPair;
    if (!checkPair(lHand, lPair)) throw std::invalid_argument("lHand does not have a pair");
    if (!checkPair(rHand, rPair)) throw std::invalid_argument("rHand does not have a pair");
    if (lPair[0].rank < rPair[0].rank) return true;
    if (lPair[0].rank > rPair[0].rank) return false;
    // Pair ranks are equal, compare non-paired cards

    // We will search for the highest non-paired card in each hand, excluding these:
    Hand lExclude(lPair.begin(), lPair.end());
    Hand rExclude(rPair.begin(), rPair.end());

    for (int i = 0; i < HoldEmGame::FLOP_COMMON_SIZE; i++) {
        Card<HoldEmRank, Suit> lMaxCard = findMaxCardNotExcluded(lHand, lExclude);
        Card<HoldEmRank, Suit> rMaxCard = findMaxCardNotExcluded(rHand, rExclude);
        if (lMaxCard.rank < rMaxCard.rank) return true;
        if (lMaxCard.rank > rMaxCard.rank) return false;

        lExclude.push_back(lMaxCard);
        rExclude.push_back(rMaxCard);
    }

    // Hands are equal
    return false;
}

/// @brief Check if a hand with a twopair is less than another hand with a twopair
/// @param lHand Sorted hand with a twopair
/// @param rHand Sorted hand with a twopair
/// @return if the rank of the higher pair of the first object is less than the rank of the higher
/// pair of the second object, or if those are the same if the rank of the lower pair of the first
/// object is less than the rank of the lower pair of the second object, or if those are also the
/// same, if the non-paired card of the first object is less than that of the second object
bool twopairLessThan(const Hand& lHand, const Hand& rHand) {
    Pair lPair1, lPair2, rPair1, rPair2;
    if (!checkTwoPair(lHand, lPair1, lPair2))
        throw std::invalid_argument("lHand does not have twopair");
    if (!checkTwoPair(rHand, rPair1, rPair2))
        throw std::invalid_argument("rHand does not have twopair");

    // Compare higher pairs
    Pair lpairH = max(lPair1, lPair2);
    Pair rPairH = max(rPair1, rPair2);

    if (lpairH[0].rank < rPairH[0].rank) return true;
    if (lpairH[0].rank > rPairH[0].rank) return false;

    // Higher pairs are equal, compare lower pairs

    Pair lpairL = min(lPair1, lPair2);
    Pair rPairL = min(rPair1, rPair2);

    if (lpairL[0].rank < rPairL[0].rank) return true;
    if (lpairL[0].rank > rPairL[0].rank) return false;

    // Lower pairs are equal, compare non-paired cards

    // We will search for the highest non-paired card in each hand, excluding these:
    Hand lExclude(lPair1.begin(), lPair1.end());
    lExclude.insert(lExclude.end(), lPair2.begin(), lPair2.end());
    Hand rExclude(rPair1.begin(), rPair1.end());
    rExclude.insert(rExclude.end(), rPair2.begin(), rPair2.end());

    Card<HoldEmRank, Suit> lMaxCard = findMaxCardNotExcluded(lHand, lExclude);
    Card<HoldEmRank, Suit> rMaxCard = findMaxCardNotExcluded(rHand, rExclude);

    if (lMaxCard.rank < rMaxCard.rank) return true;

    // Hands are equal
    return false;
}

/// @brief Check if a hand with a threeofakind is less than another hand with a threeofakind
/// @param lHand Sorted hand with a threeofakind
/// @param rHand Sorted hand with a threeofakind
/// @return if the three cards that have the same rank in the first object are of lower rank than
/// the three cards that have the same rank in the second object
bool threeofakindLessThan(const Hand& lHand, const Hand& rHand) {
    std::vector<Card<HoldEmRank, Suit>> lThreeOfAKind, rThreeOfAKind;
    if (!checkThreeOfAKind(lHand, lThreeOfAKind))
        throw std::invalid_argument("lHand does not have threeofakind");
    if (!checkThreeOfAKind(rHand, rThreeOfAKind))
        throw std::invalid_argument("rHand does not have threeofakind");

    return lThreeOfAKind[0].rank < rThreeOfAKind[0].rank;
}

/// @brief Check if a hand with a straight is less than another hand with a straight
/// @param lHand Sorted hand with a straight
/// @param rHand Sorted hand with a straight
/// @return if the highest-ranked card in the first object is less than the highest-ranked card in
/// the second object
bool straightLessThan(const Hand& lHand, const Hand& rHand) {
    Card<HoldEmRank, Suit> lHighestCard, rHighestCard;
    if (!checkStraight(lHand, lHighestCard))
        throw std::invalid_argument("lHand does not have a straight");
    if (!checkStraight(rHand, rHighestCard))
        throw std::invalid_argument("rHand does not have a straight");

    return lHighestCard.rank < rHighestCard.rank;
}

/// @brief Check if a hand with a flush or xhigh is less than another hand with a flush
/// @param lHand Sorted hand with a flush or xhigh
/// @param rHand Sorted hand with a flush
/// @return if the highest-ranked card in the first object is less than the highest-ranked card in
/// the second object, or if those are the same and the second highest-ranked card in the first
/// object is less than the second highest-ranked card in the second object, or if those are the
/// same and the third highest-ranked card in the first object is less than the third highest-ranked
/// card in the second object, or if those are the same and the fourth highest-ranked card in the
/// first object is less than the fourth highest-ranked card in the second object, or if those are
/// the same and the fifth highest-ranked card in the first object is less than the fifth
/// highest-ranked card in the second object
bool flushOrXhighLessThan(const Hand& lHand, const Hand& rHand) {
    // No need to check for hand rank, everything is xhigh

    // Compare cards from highest to lowest rank, because the input must be sorted
    for (int i = HoldEmGame::FINAL_HAND_SIZE - 1; i >= 0; i--) {
        if (lHand[i].rank < rHand[i].rank) return true;
        if (lHand[i].rank > rHand[i].rank) return false;
    }

    // Hands are equal
    return false;
}

/// @brief Check if a hand with a fullhouse is less than another hand with a fullhouse
/// @param lHand Sorted hand with a fullhouse
/// @param rHand Sorted hand with a fullhouse
/// @return if the three cards that have the same rank in the first object are of lower rank than
/// the three cards that have the same rank in the second object
bool fullhouseLessThan(const Hand& lHand, const Hand& rHand) {
    std::vector<Card<HoldEmRank, Suit>> lThreeOfAKind, rThreeOfAKind;
    if (!checkThreeOfAKind(lHand, lThreeOfAKind))
        throw std::invalid_argument("lHand does not have threeofakind");
    if (!checkThreeOfAKind(rHand, rThreeOfAKind))
        throw std::invalid_argument("rHand does not have threeofakind");

    // Compare three of a kind
    if (lThreeOfAKind[0].rank < rThreeOfAKind[0].rank) return true;
    if (lThreeOfAKind[0].rank > rThreeOfAKind[0].rank) return false;

    // Three of a kind are equal, spec does not say to compare pair
    return false;
}

bool fourofakindLessThan(const Hand& lHand, const Hand& rHand) {
    HoldEmRank lFourRank, rFourRank;
    if (!checkFourOfAKind(lHand, lFourRank))
        throw std::invalid_argument("lHand does not have fourofakind");
    if (!checkFourOfAKind(rHand, rFourRank))
        throw std::invalid_argument("rHand does not have fourofakind");

    return lFourRank < rFourRank;
}

/// @brief Check if a hand with a straightflush is less than another hand with a straightflush
/// @param lHand Sorted hand with a straightflush
/// @param rHand Sorted hand with a straightflush
/// @return if the highest-ranked card in the first object is less than the highest card in the
/// second object
bool straightflushLessThan(const Hand& lHand, const Hand& rHand) {
    if (!checkStraightFlush(lHand))
        throw std::invalid_argument("lHand does not have a straightflush");
    if (!checkStraightFlush(rHand))
        throw std::invalid_argument("rHand does not have a straightflush");

    return straightLessThan(lHand, rHand);  // Comparison is the same as a normal straight
}

}  // anonymous namespace

HoldEmHandRank HoldEmGame::holdem_hand_evaluation(const CardSet<HoldEmRank, Suit>& hand) const {
    CardSet<HoldEmRank, Suit> handCopy(hand);
    auto& cards = handCopy.*CardSet<HoldEmRank, Suit>::getCards();

    // Sort cards by rank
    std::sort(cards.begin(), cards.end(),
              [](const auto& lhs, const auto& rhs) { return compareRank(lhs, rhs); });

    if (cards.size() != HAND_SIZE + FLOP_COMMON_SIZE) return HoldEmHandRank::undefined;

    // Check combinations
    if (checkStraightFlush(cards)) {
        return HoldEmHandRank::straightflush;
    } else if (checkFourOfAKind(cards)) {
        return HoldEmHandRank::fourofakind;
    } else if (checkFullHouse(cards)) {
        return HoldEmHandRank::fullhouse;
    } else if (checkFlush(cards)) {
        return HoldEmHandRank::flush;
    } else if (checkStraight(cards)) {
        return HoldEmHandRank::straight;
    } else if (checkThreeOfAKind(cards)) {
        return HoldEmHandRank::threeofakind;
    } else if (checkTwoPair(cards)) {
        return HoldEmHandRank::twopair;
    } else if (checkPair(cards)) {
        return HoldEmHandRank::pair;
    } else {
        return HoldEmHandRank::xhigh;
    }
}

bool operator<(const HoldEmGame::Player& lhs, const HoldEmGame::Player& rhs) {
    if (lhs.handRank < rhs.handRank) return true;
    if (lhs.handRank > rhs.handRank) return false;

    // Hand ranks are equal, compare hands

    const auto& handRank = lhs.handRank;

    CardSet<HoldEmRank, Suit> lhsCopy(lhs.hand);
    CardSet<HoldEmRank, Suit> rhsCopy(rhs.hand);

    auto& lhsCards = lhsCopy.*CardSet<HoldEmRank, Suit>::getCards();
    auto& rhsCards = rhsCopy.*CardSet<HoldEmRank, Suit>::getCards();

    std::sort(lhsCards.begin(), lhsCards.end(),
              [](const auto& lhs, const auto& rhs) { return compareRank(lhs, rhs); });
    std::sort(rhsCards.begin(), rhsCards.end(),
              [](const auto& lhs, const auto& rhs) { return compareRank(lhs, rhs); });

    switch (handRank) {
        case HoldEmHandRank::pair:
            return pairLessThan(lhsCards, rhsCards);
        case HoldEmHandRank::twopair:
            return twopairLessThan(lhsCards, rhsCards);
        case HoldEmHandRank::threeofakind:
            return threeofakindLessThan(lhsCards, rhsCards);
        case HoldEmHandRank::straight:
            return straightLessThan(lhsCards, rhsCards);
        case HoldEmHandRank::flush:
        case HoldEmHandRank::xhigh:
            return flushOrXhighLessThan(lhsCards, rhsCards);
        case HoldEmHandRank::fullhouse:
            return fullhouseLessThan(lhsCards, rhsCards);
        case HoldEmHandRank::fourofakind:
            return fourofakindLessThan(lhsCards, rhsCards);
        case HoldEmHandRank::straightflush:
            return straightflushLessThan(lhsCards, rhsCards);
        case HoldEmHandRank::undefined:
            return false;
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const HoldEmHandRank& rank) {
    switch (rank) {
        case HoldEmHandRank::xhigh:
            os << "X High";
            break;
        case HoldEmHandRank::pair:
            os << "Pair";
            break;
        case HoldEmHandRank::twopair:
            os << "Two Pair";
            break;
        case HoldEmHandRank::threeofakind:
            os << "Three of a Kind";
            break;
        case HoldEmHandRank::straight:
            os << "Straight";
            break;
        case HoldEmHandRank::flush:
            os << "Flush";
            break;
        case HoldEmHandRank::fullhouse:
            os << "Full House";
            break;
        case HoldEmHandRank::fourofakind:
            os << "Four of a Kind";
            break;
        case HoldEmHandRank::straightflush:
            os << "Straight Flush";
            break;
        default:
            os << "Undefined";
            break;
    }
    return os;
}
