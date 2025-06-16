#ifndef PLAYER_IN_GAME_EXCEPTION_H
#define PLAYER_IN_GAME_EXCEPTION_H

#include <exception>
#include <iostream>
#include <string>

#define PLAYER_IN_GAME                                                                             \
    "Another user with your same username had joined this game and later left it. You can't join " \
    "the game, sorry!"

class PlayerAlreadyInGameException: public std::exception {
private:
public:
    PlayerAlreadyInGameException() {}

    const char* what() const noexcept override { return PLAYER_IN_GAME; }
};

#endif
