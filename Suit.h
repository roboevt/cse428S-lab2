#ifndef SUIT_H
#define SUIT_H

#include <iostream>

enum class Suit { clubs, diamonds, hearts, spades, undefined };

std::ostream& operator<<(std::ostream& os, const Suit& suit);

Suit& operator++(Suit& suit);

#endif  // SUIT_H