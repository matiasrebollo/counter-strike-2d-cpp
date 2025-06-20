#ifndef GAME_FULL_EXCEPTION_H
#define GAME_FULL_EXCEPTION_H

#include <exception>
#include <iostream>
#include <string>

#define GAME_FULL "The game's capacity is full. Try to join another game"

class GameFullException: public std::exception {
private:
public:
    GameFullException() {}

    const char* what() const noexcept override { return GAME_FULL; }
};

#endif
