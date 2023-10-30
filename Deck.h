// Deck.h   Eric Todd   e.todd@wustl.edu    Deck interface declaration.

#ifndef DECK_H
#define DECK_H

#include <iostream>

#include "CardSet.h"

template <typename R, typename S>
class Deck : public CardSet<R, S> {
    static constexpr int SHUFFLE_COUNT = 10;

   public:
    /// @brief Randomly shuffle the cards in this deck
    void shuffle();

    /// @brief Extract all cards from another CardSet and insert them into this deck
    /// @param other Card set to extract cards from
    void collect(CardSet<R, S>& other);
};

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE
#include "Deck.cpp"
#endif

#endif  // DECK_H