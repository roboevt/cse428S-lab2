// HoldEmGame.h   Eric Todd   e.todd@wustl.edu    HoldEmGame interface definition.

#pragma once

#include <iostream>

#include "Game.h"
#include "HoldEmDeck.h"

enum class HoldEmState { preflop, flop, turn, river, undefined };

enum class HoldEmHandRank {
    xhigh = 0,
    pair,
    twopair,
    threeofakind,
    straight,
    flush,
    fullhouse,
    fourofakind,
    straightflush,
    undefined
};

/// @brief Print a HoldEmHandRank to a stream
/// @param os Stream to print to
/// @param rank Hand rank to print
/// @return reference to os
std::ostream& operator<<(std::ostream& os, const HoldEmHandRank& rank);

class HoldEmGame : public Game {
    /// @brief Prints the current state of the game
    void printState() const;

    /// @brief Collects all cards from the players and common cards and puts them back in the deck
    void collectCards();

   protected:
    HoldEmState state;
    HoldEmDeck deck;
    std::vector<CardSet<HoldEmRank, Suit> > hands;
    CardSet<HoldEmRank, Suit> commonCards;

    /// @brief Deals cards to players and common cards based on the current state
    virtual void deal();

    /// @brief Plays rounds of the game until the user decides to quit
    /// @return 0 if the game was played successfully
    virtual int play();

    /// @brief Evaluates the hands of all players and prints thir current hand rank
    virtual void evaluate();

    HoldEmHandRank holdem_hand_evaluation(const CardSet<HoldEmRank, Suit>& hand) const;

   public:
    static constexpr int HAND_SIZE = 2;
    static constexpr int FLOP_COMMON_SIZE = 3;
    static constexpr int FINAL_HAND_SIZE = HAND_SIZE + FLOP_COMMON_SIZE;

    /// @brief Constroce a new HoldEmGame object
    /// @param numPlayers Number of players
    /// @param playerNames Array of c-strings of player names
    HoldEmGame(int numPlayers, const char** playerNames);

    struct Player{
        CardSet<HoldEmRank, Suit> hand;
        std::string name;
        HoldEmHandRank handRank;
        Player(CardSet<HoldEmRank, Suit> hand, std::string& name, HoldEmHandRank handRank) : hand(hand), name(name), handRank(handRank) {}
        // Player() = default;
    };
};

bool operator<(const HoldEmGame::Player& lhs, const HoldEmGame::Player& rhs);