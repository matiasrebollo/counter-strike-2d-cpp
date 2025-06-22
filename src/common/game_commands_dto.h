#ifndef GAME_COMMANDS_DTO_H
#define GAME_COMMANDS_DTO_H

#include <cstdint>
#include <variant>

#include "movements.h"
#include "weapon_type.h"

struct ForceStartDTO {};

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

struct DefuseBombDTO {
    bool make;
};

struct EquipPrimaryDTO {};
struct EquipSecondaryDTO {};
struct EquipKnifeDTO {};
struct EquipBombDTO {};

struct PickUpItemDTO {};

struct BuyGunDTO {
    const GunType gun;
};
struct BuyAmmoDTO {
    const bool for_primary;
};

using GameCommandDTO =
        std::variant<ForceStartDTO, MoveDTO, RotateDTO, PlayerActionDTO, DefuseBombDTO,
                     EquipPrimaryDTO, EquipSecondaryDTO, EquipKnifeDTO, EquipBombDTO, PickUpItemDTO,
                     BuyGunDTO, BuyAmmoDTO>;

#endif
