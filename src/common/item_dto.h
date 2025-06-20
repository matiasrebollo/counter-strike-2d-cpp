#ifndef ITEM_DTO_H
#define ITEM_DTO_H

#include <variant>

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
