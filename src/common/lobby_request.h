#ifndef LOBBY_REQUESTS_H
#define LOBBY_REQUESTS_H

#include <string>
#include <variant>

#include "skins.h"

struct CreateUsernameDTO {
    std::string username;
};

struct CreateGameDTO {
    std::string map_file_name;
};

struct JoinGameDTO {
    std::string gamename;
};

using LobbyRequestDTO = std::variant<CreateUsernameDTO, CreateGameDTO, JoinGameDTO>;

#endif
