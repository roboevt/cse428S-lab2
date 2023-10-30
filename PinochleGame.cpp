// PinochleGame.cpp  Eric Todd  e.todd@wustl.edu    Definitions for PinochleGame

#include "PinochleGame.h"

#include <algorithm>

PinochleGame::PinochleGame(int numPlayers, const char** playerNames) : Game(numPlayers, playerNames) {
    hands.resize(numPlayers);
}

void PinochleGame::deal() {
    deck.shuffle();
    //Distribute cards to each player
    for(int i = 0; i < PACKET_SIZE; i++) {
        for(size_t j = 0; j < hands.size(); j++) {
            if(!deck.is_empty())
                deck >> hands[j];
        }
    }
}

int PinochleGame::play() {
    // Play one round and then ask the user if they want to play again
    do {
        deck.shuffle();
        deal();
        printState();
        collectCards();
    } while(!promptUserEnd());

    return 0;
}

void PinochleGame::printState() const {
    for(size_t i = 0; i < hands.size(); i++) {
        // Print player name and hand
        std::cout << players[i] << ": ";
        hands[i].print(std::cout, PACKET_SIZE);

        // Evaluate melds
        std::vector<PinochleMelds> melds;
        suit_independent_evaluation(hands[i], melds);
        std::cout << "\n\tMelds: ";
        for(auto meld : melds) {
            std::cout << meld << ", ";
        }
        endl(std::cout);
    }
}

void PinochleGame::collectCards() {
    for(auto &hand : hands) {
        while(!hand.is_empty()) {
            hand >> deck;
        }
    }
}

void PinochleGame::suit_independent_evaluation(const CardSet<PinochleRank, Suit>& hand, std::vector<PinochleMelds>& melds) const {
    CardSet<PinochleRank, Suit> handCopy(hand);
    auto& cards = handCopy.*CardSet<PinochleRank, Suit>::getCards();

    // Sort the cards by rank
    std::sort(cards.begin(), cards.end(), [](const auto& lhs, const auto& rhs) {
        return compareRank(lhs, rhs);
    });
    handCopy.print(std::cout, PACKET_SIZE);

    // Helper lambdas for checking if the hand has certain card combinations

    const auto hand_has_card = [&cards](PinochleRank rank, Suit suit) {
        return std::find(cards.begin(), cards.end(), Card<PinochleRank, Suit>(rank, suit)) != cards.end();
    };

    const auto hand_has_both_cards = [&cards](PinochleRank rank, Suit suit) {
        const auto goal = std::vector<Card<PinochleRank, Suit>>(2, Card<PinochleRank, Suit>(rank, suit));
        return std::includes(cards.begin(), cards.end(), goal.begin(), goal.end());
    };

    const auto hand_has_one_of_each_suit = [&cards, &hand_has_card](PinochleRank rank) {
        return hand_has_card(rank, Suit::spades) && 
               hand_has_card(rank, Suit::hearts) && 
               hand_has_card(rank, Suit::clubs)  && 
               hand_has_card(rank, Suit::diamonds);
    };

    const auto hand_has_both_of_each_suit = [&cards, &hand_has_both_cards](PinochleRank rank) {
        return hand_has_both_cards(rank, Suit::spades) && 
               hand_has_both_cards(rank, Suit::hearts) && 
               hand_has_both_cards(rank, Suit::clubs)  && 
               hand_has_both_cards(rank, Suit::diamonds);
    };

    // Check for melds

    if(hand_has_both_of_each_suit(PinochleRank::ace)) { // Thousand Aces
        melds.push_back(PinochleMelds::thousandaces);
    } else if(hand_has_one_of_each_suit(PinochleRank::ace)) { // Hundred Aces
        melds.push_back(PinochleMelds::hundredaces);
    }

    if(hand_has_both_of_each_suit(PinochleRank::king)) { // Eight Hundred Kings
        melds.push_back(PinochleMelds::eighthundredkings);
    } else if(hand_has_one_of_each_suit(PinochleRank::king)) { // Eighty Kings
        melds.push_back(PinochleMelds::eightykings);
    }

    if(hand_has_both_of_each_suit(PinochleRank::queen)) { // Six Hundred Queens
        melds.push_back(PinochleMelds::sixhundredqueens);
    } else if(hand_has_one_of_each_suit(PinochleRank::queen)) { // Sixty Queens
        melds.push_back(PinochleMelds::sixtyqueens);
    }

    if(hand_has_both_of_each_suit(PinochleRank::jack)) { // Four Hundred Jacks
        melds.push_back(PinochleMelds::fourhundredjacks);
    } else if(hand_has_one_of_each_suit(PinochleRank::jack)) { // Forty Jacks
        melds.push_back(PinochleMelds::fortyjacks);
    }

    if(hand_has_both_cards(PinochleRank::jack, Suit::diamonds) && 
            hand_has_both_cards(PinochleRank::queen, Suit::spades)) { // Double Pinochle
        melds.push_back(PinochleMelds::doublepinochle);
    } else if(hand_has_card(PinochleRank::jack, Suit::diamonds) &&
            hand_has_card(PinochleRank::queen, Suit::spades)) { // Pinochle
        melds.push_back(PinochleMelds::pinochle);
    }
}


std::ostream& operator<<(std::ostream& os, const PinochleMelds& meld) {
    switch(meld) {
        case PinochleMelds::dix:
            os << "Dix";
            break;
        case PinochleMelds::offsuitmarriage:
            os << "Offsuit Marriage";
            break;
        case PinochleMelds::fortyjacks:
            os << "Forty Jacks";
            break;
        case PinochleMelds::pinochle:
            os << "Pinochle";
            break;
        case PinochleMelds::insuitmarriage:
            os << "Insuit Marriage";
            break;
        case PinochleMelds::sixtyqueens:
            os << "Sixty Queens";
            break;
        case PinochleMelds::eightykings:
            os << "Eighty Kings";
            break;
        case PinochleMelds::hundredaces:
            os << "Hundred Aces";
            break;
        case PinochleMelds::insuitrun:
            os << "Insuit Run";
            break;
        case PinochleMelds::doublepinochle:
            os << "Double Pinochle";
            break;
        case PinochleMelds::fourhundredjacks:
            os << "Four Hundred Jacks";
            break;
        case PinochleMelds::sixhundredqueens:
            os << "Six Hundred Queens";
            break;
        case PinochleMelds::eighthundredkings:
            os << "Eight Hundred Kings";
            break;
        case PinochleMelds::thousandaces:
            os << "Thousand Aces";
            break;
        case PinochleMelds::insuitdoublerun:
            os << "Insuit Double Run";
            break;
        default:
            os << "Unknown Meld";
            break;
    }
    // Point value
    os << " " << PinochleGame::POINT_VALUES[static_cast<int>(meld)] << " points";
    return os;
}