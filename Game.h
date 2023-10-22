// Game.h   Eric Todd   e.todd@wustl.edu    Game interface definition.

#pragma once

#include <string>
#include <string_view>
#include <vector>

class Game {
   protected:
    std::vector<std::string> players;

    /// @brief Ask the user is they would like to end the game
    /// @return Whether they want to end the game
    bool promptUserEnd() const;

   public:
    Game(int numPlayers, const char** playerNames);
    virtual int play() = 0;

    // String that user should enter to confirm they want to end the game.
    static constexpr std::string_view USER_CONFIRMATION = "yes";
};