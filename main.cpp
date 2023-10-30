#include <iostream>
#include <memory>
#include <string_view>

#include "HoldEmGame.h"
#include "PinochleGame.h"

constexpr int BASIC_ARGS = 2;  // To skip program name[0] and game name[1]
constexpr int GAME_NAME_ARG = 1;

constexpr int PINOCHLE_PLAYERS = 4;
constexpr int HOLDEM_MIN_PLAYERS = 2;
constexpr int HOLDEM_MAX_PLAYERS = 9;

constexpr std::string_view PINOCHLE_GAME_NAME = "Pinochle";
constexpr std::string_view HOLDEM_GAME_NAME = "HoldEm";

constexpr int SUCCESS = 0;
constexpr int INVALID_ARGS = 1;

void usage() {
    std::cout << "Usage: lab2 <Game(Pinochle|HoldEm)> <player1> <player2> ... <player9>"
              << std::endl
              << "Pinocle requires 4 players, HoldEm requires 2-9 players" << std::endl;
}

bool checkArgs(int argc, const char** argv) {
    if (argc < BASIC_ARGS) {
        usage();
        return false;
    }
    if (argv[GAME_NAME_ARG] == PINOCHLE_GAME_NAME && argc == PINOCHLE_PLAYERS + BASIC_ARGS) {
        return true;
    }
    if (argv[GAME_NAME_ARG] == HOLDEM_GAME_NAME && argc >= HOLDEM_MIN_PLAYERS + BASIC_ARGS &&
        argc <= HOLDEM_MAX_PLAYERS + BASIC_ARGS) {
        return true;
    }
    usage();
    return false;
}

std::shared_ptr<Game> create(std::string_view gameName, int numPlayers, const char** playerNames) {
    if (gameName == PINOCHLE_GAME_NAME) {
        return std::make_shared<PinochleGame>(numPlayers, playerNames);
    }
    if (gameName == HOLDEM_GAME_NAME) {
        return std::make_shared<HoldEmGame>(numPlayers, playerNames);
    }
    return nullptr;
}

auto main(int argc, const char** argv) -> int {
    if (checkArgs(argc, argv)) {
        std::shared_ptr<Game> game =
            create(argv[GAME_NAME_ARG], argc - BASIC_ARGS, argv + BASIC_ARGS);
        if (game)
            return game->play();
        else {
            usage();
            return INVALID_ARGS;
        }
    }
    return INVALID_ARGS;
}