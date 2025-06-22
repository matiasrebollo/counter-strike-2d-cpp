#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include "commands.h"

enum ResponseStatus {
    SUCCESS = 0,
    GAME_NOT_CREATED = 1,
    USERNAME_IN_USE = 2,
    GAME_NOT_EXIST = 3,
    GAME_ALREADY_STARTED = 4,
    GAME_IS_DEAD = 5,
    GAME_IS_FULL = 6,
    USERNAME_ALREADY_IN_GAME = 7,
    WITHOUT_USERNAME = 8,
};

struct ServerResponseLobby {
    CommandType commandType;
    ResponseStatus status;
    std::string game_name;
};

#endif
