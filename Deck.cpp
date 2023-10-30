// Deck.cpp    Eric Todd   e.todd@wustl.edu    Definitions for Deck shuffle and collect function.

#include "Deck.h"

#include <algorithm>
#include <random>

template <typename R, typename S>
void Deck<R, S>::shuffle() {
    std::random_device rng;
    std::mt19937 gen(rng());

    // Shuffle the deck SHUFFLE_COUNT times for good measure
    for (int i = 0; i < SHUFFLE_COUNT; i++) {
        std::shuffle(CardSet<R, S>::cards.begin(), CardSet<R, S>::cards.end(), gen);
    }
}

template <typename R, typename S>
void Deck<R, S>::collect(CardSet<R, S>& other) {
    while (!other.is_empty()) {
        other >> *this;
    }
}