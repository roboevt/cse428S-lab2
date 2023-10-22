//HoldEmDeck.cpp    Eric Todd   e.todd@wustl.edu    Definitions for HoldEmGame constructor, 
// insertion operator, and increment

#include "HoldEmDeck.h"

HoldEmDeck::HoldEmDeck() {
    for (HoldEmRank rank = HoldEmRank::two; rank < HoldEmRank::undefined; ++rank) {
        for (Suit suit = Suit::clubs; suit < Suit::undefined; ++suit) {
            cards.push_back(Card<HoldEmRank, Suit>(rank, suit));
        }
    }
}

std::ostream& operator<<(std::ostream& os, const HoldEmRank& rank) {
    if (rank <= HoldEmRank::ten) {  // 2-10 enum values match labels
        os << static_cast<int>(rank);
    } else
        switch (rank) {
            case HoldEmRank::jack:
                os << "J";
                break;
            case HoldEmRank::queen:
                os << "Q";
                break;
            case HoldEmRank::king:
                os << "K";
                break;
            case HoldEmRank::ace:
                os << "A";
                break;
            default:
                os << "?";
                break;
        }
    return os;
}

HoldEmRank& operator++(HoldEmRank& rank) {
    if (rank < HoldEmRank::undefined) { // Don't overincremnt rank
        // Increment the int backing the enum
        rank = static_cast<HoldEmRank>(static_cast<int>(rank) + 1);
    }
    return rank;
}