#ifndef PLAYER_DTO_H
#define PLAYER_DTO_H

#include <cstdint>
#include <string>

#include "common/vector_2d.h"
#include "common/weapon_type.h"

struct PlayerDTO {
    std::string username;
    Vector2D position;
    double orientation;
    /*GunType primary_gun;
    uint16_t primary_ammo;
    GunType secondary_gun;
    uint16_t secondary_ammo;
    WeaponType equipped;
    bool is_shooting; // coordenadas de impacto del tiro??
    bool is_getting_hit;*/
    uint16_t life;
    // bool has_bomb;
};


#endif
