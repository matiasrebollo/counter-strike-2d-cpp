#ifndef LOADOUT_DTO_H
#define LOADOUT_DTO_H

#include <cstdint>
#include <string>

#include "common/weapon_type.h"

struct LoadoutDTO {
    uint16_t money;
    GunType primary_gun;
    uint16_t primary_ammo;
    GunType secondary_gun;
    uint16_t secondary_ammo;
    WeaponType equipped;
    // bool is_shooting;
    // bool is_getting_hit;
    // bool has_bomb;
};


#endif
