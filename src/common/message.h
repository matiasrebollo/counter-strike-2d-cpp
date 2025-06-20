#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include "commands.h"

enum ResponseStatus {
    SUCCESS = 0,
    GAME_NOT_CREATED = 1,
    USERNAME_IN_USE = 2,
    GAME_NOT_EXIST = 3,
    GAME_IS_DEAD = 4,
    GAME_IS_FULL = 5,
    USERNAME_ALREADY_IN_GAME = 6,
    WITHOUT_USERNAME = 7,
};

struct ServerResponseLobby {
    CommandType commandType;
    ResponseStatus status;
    std::string game_name;
};

#endif
