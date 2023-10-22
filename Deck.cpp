//Deck.cpp    Eric Todd   e.todd@wustl.edu    Definitions for Deck shuffle and collect function.

#include "Deck.h"

#include <random>
#include <algorithm>

template <typename R, typename S>
void Deck<R,S>::shuffle() {
    std::random_device rng;
    std::mt19937 gen(rng());

    std::shuffle(CardSet<R,S>::cards.begin(), CardSet<R,S>::cards.end(), gen);
}

template <typename R, typename S>
void Deck<R,S>::collect(CardSet<R, S>& other) {
    while(!other.is_empty()) {
        other >> *this;
    }
}