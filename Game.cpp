// HoldEmDeck.cpp    Eric Todd   e.todd@wustl.edu    Definitions for Game constructor, 
// and prompt function

#include "Game.h"

#include <iostream>

Game::Game(int numPlayers, const char** playerNames) {
    for(int i = 0; i < numPlayers; i++) {
        players.push_back(playerNames[i]);
    }
}

bool Game::promptUserEnd() const {
    std::string userResponse;
    std::cout << "Would you like to end the game? (yes/no): ";
    std::cin >> userResponse;
    return userResponse == USER_CONFIRMATION;
}