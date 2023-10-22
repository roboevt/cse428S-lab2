// HoldEmDeck.h   Eric Todd   e.todd@wustl.edu    HoldEmDeck.h interface definition.

#ifndef HOLD_EM_DECK_H
#define HOLD_EM_DECK_H

#include <iostream>
#include <vector>

#include "Card.h"
#include "Deck.h"
#include "Suit.h"

enum class HoldEmRank {
    two = 2,  // rank values match their numeric face values
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    jack,
    queen,
    king,
    ace,
    undefined
};

class HoldEmDeck : public Deck<HoldEmRank, Suit> {
    static constexpr int PRINT_ROW_SIZE = 4;  // number of cards to print per row

   public:
    HoldEmDeck();
};

/// @brief Print a HoldEmRank to a stream
/// @param os Out stream to write to
/// @param rank Rank to be printed
/// @return A reference to the ostream for chaining
std::ostream& operator<<(std::ostream& os, const HoldEmRank& rank);

/// @brief Increment HoldEmRank
/// @param rank Rank to be increment
/// @return Incremented rank
HoldEmRank& operator++(HoldEmRank& rank);

#endif  // HOLD_EM_DECK_H