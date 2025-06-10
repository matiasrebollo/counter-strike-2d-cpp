#ifndef COMMANDS_DTO_H
#define COMMANDS_DTO_H

#include <cstdint>
#include <variant>

#include "movements.h"
#include "weapon_type.h"

struct MoveDTO {
    Movement dir;
    bool move;
};
struct RotateDTO {
    const double angle;
};

struct PlayerActionDTO {
    bool make;
};
struct EquipPrimaryDTO {};
struct EquipSecondaryDTO {};
struct EquipKnifeDTO {};
struct EquipBombDTO {};

struct BuyGunDTO {
    const GunType gun;
};
struct BuyAmmoDTO {
    const bool for_primary;
};

using CommandDTO =
        std::variant<MoveDTO, RotateDTO, PlayerActionDTO, EquipPrimaryDTO, EquipSecondaryDTO,
                     EquipKnifeDTO, EquipBombDTO, BuyGunDTO, BuyAmmoDTO>;

#endif
