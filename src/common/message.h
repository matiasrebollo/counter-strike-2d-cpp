#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include "commands.h"

struct ServerResponseLobby {
    CommandType commandType;
    bool success;
    std::string game_name;
};

#endif
