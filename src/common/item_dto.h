#ifndef ITEM_DTO_H
#define ITEM_DTO_H

#include <cstdint>
#include <variant>

#include "common/vector_2d.h"
#include "common/weapon_type.h"

struct DroppedGunDTO {
    Vector2D<int> position;
    GunType gun_type;
    uint16_t ammo;
};

struct DroppedBombDTO {
    Vector2D<int> position;
};


using ItemDTO = std::variant<DroppedGunDTO, DroppedBombDTO>;

#endif
