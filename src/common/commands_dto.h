#ifndef COMMANDS_DTO_H
#define COMMANDS_DTO_H

#include <cstdint>
#include <variant>

#include "weapon_type.h"

struct MoveUpDTO {};
struct MoveDownDTO {};
struct MoveLeftDTO {};
struct MoveRightDTO {};
struct RotateDTO {
    const double angle;
};

struct PlayerActionDTO {};
struct EquipPrimaryDTO {};
struct EquipSecondaryDTO {};
struct EquipKnifeDTO {};
struct EquipBombDTO {};

struct BuyGunDTO {
    const GunType gun;
};
struct BuyAmmoDTO {
    const uint16_t ammo;
    const bool for_primary;
};

using CommandDTO = std::variant<MoveUpDTO, MoveDownDTO, MoveLeftDTO, MoveRightDTO, RotateDTO,
                                PlayerActionDTO, EquipPrimaryDTO, EquipSecondaryDTO, EquipKnifeDTO,
                                EquipBombDTO, BuyGunDTO, BuyAmmoDTO>;

#endif
