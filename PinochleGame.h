// PinochleGame.h   Eric Todd   e.todd@wustl.edu    PinochleGame interface definition.

#pragma once

#include "Game.h"
#include "PinochleDeck.h"

#include <iostream>

enum class PinochleMelds {
    dix = 0,  // Rest of the values are assigned automatically to be sequential
    offsuitmarriage,  // See PinochleGame::POINT_VALUES for motivation
    fortyjacks,
    pinochle,
    insuitmarriage,
    sixtyqueens,
    eightykings,
    hundredaces,
    insuitrun,
    doublepinochle,
    fourhundredjacks,
    sixhundredqueens,
    eighthundredkings,
    thousandaces,
    insuitdoublerun
};

/// @brief Print a meld name and point value to a stream
/// @param os The stream to print to
/// @param meld The meld to print
/// @return Reference to os
std::ostream& operator<<(std::ostream& os, const PinochleMelds& meld);

class PinochleGame : public Game {
    /// @brief Prints the state of the game to the console.
    void printState() const;

    /// @brief Collects all cards from the players and puts them back in the deck.
    void collectCards();

    /// @brief Evaluates the hands of all players and prints their current hand rank.
    /// @param hand The hand to evaluate
    /// @param melds Output parameter to store the melds in
    void suit_independent_evaluation(const CardSet<PinochleRank, Suit>& hand, std::vector<PinochleMelds>& melds) const;

    static constexpr int PACKET_SIZE = 30;

   protected:
    PinochleDeck deck;
    std::vector<CardSet<PinochleRank, Suit> > hands;

    /// @brief Deals cards to players
    virtual void deal();

   public:
    /// @brief Constructs a new PinochleGame object
    /// @param numPlayers Number of players
    /// @param playerNames Array of c-strings of player names
    PinochleGame(int numPlayers, const char** playerNames);

    /// @brief Plays rounds of the game until the user decides to quit
    virtual int play();

    /// @brief Index this array with PinochleMelds for point values associated with each label
    static constexpr int POINT_VALUES[] = {
        10, 20, 40, 40, 40, 60, 80, 100, 150, 300, 400, 600, 800, 1000, 1500
    };
};