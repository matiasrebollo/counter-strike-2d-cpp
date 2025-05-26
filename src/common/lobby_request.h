#ifndef LOBBY_REQUESTS_H
#define LOBBY_REQUESTS_H

#include <string>
#include <variant>

#include "skins.h"

struct CreateUsernameDTO {
    std::string username;
};

struct CreateGameDTO {
    int size_players;
    TerroristSkin tt_skin;
    CounterTerroristSkin ct_skin;
};

struct JoinGameDTO {
    std::string gamename;
    TerroristSkin tt_skin;
    CounterTerroristSkin ct_skin;
};

using LobbyRequestDTO = std::variant<CreateUsernameDTO, CreateGameDTO, JoinGameDTO>;

#endif
