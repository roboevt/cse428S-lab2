// CardSet.cpp    Eric Todd   e.todd@wustl.edu    Definitions for Card Set print, is_empty, and extraction.

#include "CardSet.h"

template<typename R, typename S>
void CardSet<R, S>::print(std::ostream& os, size_t n) const {
    for(size_t i = 0; i < cards.size(); i++) {
        os << cards[i] << " ";
        // Print a newline every n cards
        if(((i + 1) % n) == 0) endl(os);
    }
}

template<typename R, typename S>
bool CardSet<R, S>::is_empty() const {
    return cards.empty();
}

template<typename R, typename S>
CardSet<R, S>& CardSet<R, S>::operator>>(CardSet<R, S>& other) {
    if(is_empty()) throw std::runtime_error("Cannot move cards from empty set");

    other.cards.push_back(cards.back());
    cards.pop_back();

    return *this;
}

template<typename R, typename S>
std::vector<Card<R, S> > CardSet<R, S>::*CardSet<R, S>::getCards() {  // What a signature
    return &CardSet::cards;
}

// Deep copy constructor (vector's copy constructor is deep)
template<typename R, typename S>
CardSet<R, S>::CardSet(const CardSet<R, S>& other) : cards(other.cards) {}
