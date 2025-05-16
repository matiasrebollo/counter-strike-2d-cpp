#ifndef PLAYER_DTO_H
#define PLAYER_DTO_H

#include <cstdint>

#include "common/vector_2d.h"
#include "common/weapon_type.h"

struct PlayerDTO {
    const Vector2D position;
    const Vector2D direction;
    /*const GunType primary_gun;
    const uint16_t primary_ammo;
    const GunType secondary_gun;
    const uint16_t secondary_ammo;
    const WeaponType equipped;
    const bool is_shooting; // coordenadas de impacto del tiro??
    const bool is_getting_hit;*/
    const uint16_t life;
    // const bool has_bomb;
};


#endif
