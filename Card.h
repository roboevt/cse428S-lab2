// Card.h   Eric Todd   e.todd@wustl.edu    Card interface declaration.

#ifndef CARD_H
#define CARD_H

#include <iostream>

template <typename R, typename S> struct Card;

/// @brief Print a card to a stream
/// @tparam R Rank of card
/// @tparam S Suit of card
/// @param os Output stream to write to
/// @param card Card to print
/// @return reference to os
template <typename R, typename S>
std::ostream& operator<<(std::ostream& os, const Card<R, S>& card);

template <typename R, typename S>
struct Card {
    R rank;
    S suit;
    Card(R rank, S suit);
    Card() = default;
};

template <typename R, typename S>
bool compareRank(const Card<R, S>& lhs, const Card<R, S>& rhs);

template <typename R, typename S>
bool compareSuit(const Card<R, S>& lhs, const Card<R, S>& rhs);

template <typename R, typename S>
bool operator<(const Card<R, S>& lhs, const Card<R, S>& rhs) {
    return compareRank(lhs, rhs);
}

template <typename R, typename S>
bool operator==(const Card<R, S>& lhs, const Card<R, S>& rhs) {
    return lhs.rank == rhs.rank && lhs.suit == rhs.suit;
}

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE
#include "Card.cpp"
#endif

#endif  // CARD_H