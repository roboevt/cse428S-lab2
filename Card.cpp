// Card.cpp    Eric Todd   e.todd@wustl.edu    Definitions for templated Card constructor and insertion operator.

#include "Card.h"

template <typename R, typename S>
Card<R,S>::Card(R rank, S suit) : rank(rank), suit(suit) {}

template <typename R, typename S>
std::ostream& operator<<(std::ostream& os, const Card<R, S>& card) {
    os << card.rank << card.suit;
    return os;
}

template <typename R, typename S>
bool compareRank(const Card<R, S>& lhs, const Card<R, S>& rhs) {
    if(lhs.rank == rhs.rank) {
        return lhs.suit < rhs.suit;
    }else {
        return lhs.rank < rhs.rank;
    }
}

template <typename R, typename S>
bool compareSuit(const Card<R, S>& lhs, const Card<R, S>& rhs) {
    if(lhs.suit == rhs.suit) {
        return lhs.rank < rhs.rank;
    }else {
        return lhs.suit < rhs.suit;
    }
}