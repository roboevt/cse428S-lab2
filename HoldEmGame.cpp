// HoldEmGame.cpp   Eric Todd   e.todd@wustl.edu    Definitions for HoldEmGame class. Constructor,
// dealing, and turn handling.

#include "HoldEmGame.h"

#include <algorithm>

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
        }
        collectCards();
    } while (!promptUserEnd());

    return 0;
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
    for (auto& hand : hands) {
        while (!hand.is_empty()) {
            hand >> deck;
        }
    }
    while (!commonCards.is_empty()) {
        commonCards >> deck;
    }
}

// Anonymous namespace for helper functions
namespace {
/// @brief Check a hand for a straight flush
/// @param hand The hand to check sorted by rank
/// @return if all five cards are of the same suit and their ranks are consecutive (with the
/// special rule that A 2 3 4 5 of the same suit and 10 J Q K A of the same suit are the lowest and
/// highest valid straight flushes respectively, but that any hand with cards ranked K A 2 is not
/// considered a straight flush)
bool checkStraightFlush(const std::vector<Card<HoldEmRank, Suit>>& hand) {
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
bool checkFourOfAKind(const std::vector<Card<HoldEmRank, Suit>>& hand) {
    bool isFourOfAKind = false;

    // Two cases, x x x x y or x y y y y

    if (hand[0].rank == hand[3].rank || hand[1].rank == hand[4].rank) {
        isFourOfAKind = true;
    }

    return isFourOfAKind;
}

/// @brief Check a hand for a full house
/// @param hand The hand to check sorted by rank
/// @return if three cards have the same rank and the other two cards have the same rank
bool checkFullHouse(const std::vector<Card<HoldEmRank, Suit>>& hand) {
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
bool checkFlush(const std::vector<Card<HoldEmRank, Suit>>& hand) {
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
bool checkStraight(const std::vector<Card<HoldEmRank, Suit>>& hand) {
    int rank = static_cast<int>(hand[0].rank);
    bool isStraight = true;
    for (size_t i = 1; i < hand.size(); i++) {
        if (static_cast<int>(hand[i].rank) != rank + 1) {
            isStraight = false;
            break;
        }
        rank++;
    }

    // Check for ace low straight
    if (!isStraight && hand[0].rank == HoldEmRank::ace) {
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

    return isStraight;
}

/// @brief Check a hand for three of a kind
/// @param hand The hand to check sorted by rank
/// @return if three cards have the same rank and the other two cards are of different ranks than any other cards in the hand
bool checkThreeOfAKind(const std::vector<Card<HoldEmRank, Suit>>& hand) {
    if(checkFourOfAKind(hand)) return false; // The other two cards must be different ranks

    bool isThreeOfAKind = false;

    // Three cases, x x x y z, x y y y z, x y z z z

    if (hand[0].rank == hand[2].rank) {
        isThreeOfAKind = true;
    } else if (hand[1].rank == hand[3].rank) {
        isThreeOfAKind = true;
    } else if (hand[2].rank == hand[4].rank) {
        isThreeOfAKind = true;
    }

    return isThreeOfAKind;
}

/// @brief Check a hand for two pair
/// @param hand The hand to check sorted by rank
/// @return if two cards have the same rank and two other cards share a different rank and the fifth card is of yet another rank
bool checkTwoPair(const std::vector<Card<HoldEmRank, Suit>>& hand) {
    // Both pairs must be different and the other card must be different
    if(checkFourOfAKind(hand) || checkFullHouse(hand)) return false;

    bool isTwoPair = false;

    // Three cases, x x y y z, x y y z z, x x y z z

    if (hand[0].rank == hand[1].rank && hand[2].rank == hand[3].rank) {
        isTwoPair = true;
    } else if (hand[1].rank == hand[2].rank && hand[3].rank == hand[4].rank) {
        isTwoPair = true;
    } else if (hand[0].rank == hand[1].rank && hand[3].rank == hand[4].rank) {
        isTwoPair = true;
    }

    return isTwoPair;
}

/// @brief Check a hand for a pair
/// @param hand The hand to check sorted by rank
/// @return if only two cards in the hand have the same rank
bool checkPair(const std::vector<Card<HoldEmRank, Suit>>& hand) {
    // The other three cards must be different ranks
    if(checkFourOfAKind(hand) || checkFullHouse(hand) || checkThreeOfAKind(hand) || checkTwoPair(hand)) return false;

    bool isPair = false;

    // Four cases, x x y z w, x y y z w, x y z z w, x y z w w

    if (hand[0].rank == hand[1].rank) {
        isPair = true;
    } else if (hand[1].rank == hand[2].rank) {
        isPair = true;
    } else if (hand[2].rank == hand[3].rank) {
        isPair = true;
    } else if (hand[3].rank == hand[4].rank) {
        isPair = true;
    }

    return isPair;
}

}  // namespace

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