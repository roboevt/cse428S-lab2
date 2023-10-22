// PinochleDeck.h   Eric Todd   e.todd@wustl.edu    PinochleDeck.h interface definition.

#ifndef PINOCHLEDECK_H
#define PINOCHLEDECK_H

#include <iostream>
#include <vector>

#include "Card.h"
#include "Deck.h"
#include "Suit.h"

enum class PinochleRank { nine, jack, queen, king, ten, ace, undefined };

class PinochleDeck : public Deck<PinochleRank, Suit> {
    static constexpr int PRINT_ROW_SIZE = 4;  // number of cards to print per row
    static constexpr int NUM_DECKS = 2;       // number of decks to use

   public:
    PinochleDeck();
};

/// @brief Print a PinochleRank to a stream
/// @param os Out stream to write to
/// @param rank Rank to be printed
/// @return A reference to the ostream for chaining
std::ostream& operator<<(std::ostream& os, const PinochleRank& rank);

/// @brief Increment PinochleRank
/// @param rank Rank to be increment
/// @return Incremented rank
PinochleRank& operator++(PinochleRank& rank);

#endif  // PINOCHLEDECK_H