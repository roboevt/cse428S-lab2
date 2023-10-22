// CardSet.h   Eric Todd   e.todd@wustl.edu    CardSet interface declaration.

#pragma once

#include <vector>

#include "Card.h"

template <typename R, typename S>
class CardSet {
   protected:
    std::vector<Card<R, S> > cards;

   public:
    /// @brief Print this set of cards to a stream
    /// @param os OutStream to write to
    /// @param n How many cards to print on each row
    void print(std::ostream& os, size_t n) const;

    /// @brief Check if this card set is empty
    /// @return If the card set is empty
    bool is_empty() const;

    /// @brief Extract one card from this set and insert it in another one
    /// @param other Card set to insert into
    /// @return Reference to this, for chaining
    CardSet<R, S>& operator>>(CardSet<R, S>& other);

    /// @brief Get a pointer to the cards vector member variable
    /// @return Pointer to the cards vector
    static std::vector<Card<R, S> > CardSet::*getCards();

    /// @brief Copy constructor
    /// @param other Card set to copy
    CardSet<R, S>(const CardSet<R, S>& other);

    /// @brief Default constructor
    CardSet<R, S>() = default;
};

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE
#include "CardSet.cpp"
#endif