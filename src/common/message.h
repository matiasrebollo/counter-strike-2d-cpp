#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>
#include <string>
#include <vector>

#include "commands.h"
#include "movements.h"
#include "skins.h"
#include "weapons.h"

struct InternalMessage {
    uint8_t code_message;
    std::string s;
    int pos_x;
    int pos_y;
    int size_players;
    uint8_t direction;
    uint8_t code_weapon_type;
    uint8_t code_weapon;
    uint8_t skin_id_tt;
    uint8_t skin_id_ct;
    uint8_t map_id;
    uint16_t bullets;
    uint8_t movement;
};

struct MessageFromServer {
    bool haveGame;
    std::string extraMessage;
    std::vector<std::string> listGames;
};

struct ServerResponseMessage {
    uint8_t code;
    uint8_t success;
};

struct ServerResponseLobby {
    CommandType commandType;
    bool success;
    std::string game_name;
};

struct MessageFromClient {
    CommandType commandType;
    std::string s;
    Weapon weapon;
    WeaponType weaponType;
    TerroristSkin tt_skin;
    CounterTerroristSkin ct_skin;
    // Me falta lo del map, el tema es que al leerlos del yaml hago un enum?
    // no sé si es posible. TO DO: averiguar
    Movement movement;
    int size_players;
    int pos_x;
    int pos_y;
    int bullets;
    bool isOff;
};

#endif
