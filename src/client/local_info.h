#ifndef LOCAL_INFO_H
#define LOCAL_INFO_H

#include <string>
#include <unordered_map>

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
    WeaponType equipped = SECONDARY;
    bool in_site = false;
    bool has_bomb = false;
    bool movement = false;
    bool shoot = false;
    int impact_position_x = 0;
    int impact_position_y = 0;
};

struct LocalInfo {
    std::string username;
    std::string gamename;

    CounterTerroristSkin ct_skin;
    TerroristSkin tt_skin;

    std::unordered_map<std::string, PlayerInfo> players;
    PlayerInfo player;
    std::optional<Team> current_round_winner;
    BombStatus bomb_status = BombStatus::NOT_PLANTED;
    Phase phase = Phase::WAITING_PLAYERS;
    int time_left = 0;
    int total_players = 0;
    size_t total_rounds = 0;
    size_t current_round = 0;
};

#endif
