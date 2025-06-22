#ifndef GAME_ALREADY_STARTED_EXCEPTION_H
#define GAME_ALREADY_STARTED_EXCEPTION_H

#include <exception>
#include <iostream>
#include <string>

#define MSG_GAME_ALREADY_STARTED "The player have already started!"

class GameAlreadyStartedException: public std::exception {
private:
public:
    GameAlreadyStartedException() {}

    const char* what() const noexcept override { return MSG_GAME_ALREADY_STARTED; }
};

#endif
