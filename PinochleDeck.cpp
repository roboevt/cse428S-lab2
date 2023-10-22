#include "PinochleDeck.h"

PinochleDeck::PinochleDeck() {
    for (int i = 0; i < NUM_DECKS; i++) {
        for (PinochleRank rank = PinochleRank::nine; rank < PinochleRank::undefined; ++rank) {
            for (Suit suit = Suit::clubs; suit < Suit::undefined; ++suit) {
                cards.push_back(Card<PinochleRank, Suit>(rank, suit));
            }
        }
    }
}

std::ostream& operator<<(std::ostream& os, const PinochleRank& rank) {
    switch (rank) {
        case PinochleRank::nine:
            os << "9";
            break;
        case PinochleRank::ten:
            os << "T";
            break;
        case PinochleRank::jack:
            os << "J";
            break;
        case PinochleRank::queen:
            os << "Q";
            break;
        case PinochleRank::king:
            os << "K";
            break;
        case PinochleRank::ace:
            os << "A";
            break;
        default:
            os << "?";
            break;
    }
    return os;
}

PinochleRank& operator++(PinochleRank& rank) {
    if (rank < PinochleRank::undefined) {
        rank = static_cast<PinochleRank>(static_cast<int>(rank) + 1);
    }
    return rank;
}