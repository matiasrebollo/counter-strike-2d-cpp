#ifndef GAME_DEAD_EXCEPTION_H
#define GAME_DEAD_EXCEPTION_H

#include <exception>
#include <iostream>
#include <string>

#define GAME_DEAD "All the players of this games are disconnected. The game is dead!"

class GameDeadException: public std::exception {
private:
public:
    GameDeadException() {}

    const char* what() const noexcept override { return GAME_DEAD; }
};

#endif
