#ifndef LOCAL_INFO_H
#define LOCAL_INFO_H

#include <string>
#include <vector>

#include "../common/player_dto.h"
#include "../common/skins.h"

struct PlayerInfo {
    bool is_ct = false;
    int x = 0;
    int y = 0;
    double orientation = 270;
    int life = 0;
    int money = 0;
    int equipped_gun_ammo = 0;
    GunType primary_gun = NONE;
    GunType secondary_gun = NONE;
};

struct LocalInfo {
    std::string username;
    std::string gamename;

    CounterTerroristSkin ct_skin;
    TerroristSkin tt_skin;

    // hay cosas de los demas players que para graficar no me importan (como su vida, dinero)
    // ver si en lugar de tener un vector de PlayerDTO, tener un vector de ..... pero que tenga las
    // cosas que necesite
    std::vector<PlayerDTO> ct_players;
    std::vector<PlayerDTO> tt_players;

    PlayerInfo player;
    int time_left = 0;
    int total_players = 0;
};

#endif
