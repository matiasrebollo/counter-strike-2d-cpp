#ifndef WEAPON_PARSER_H
#define WEAPON_PARSER_H

#include <cstdint>
#include <string>

#include "weapon_type.h"

#define NONE_WEAPON_BINARY_CODE 0x00
#define GLOCK_WEAPON_BINARY_CODE 0x01
#define AK47_WEAPON_BINARY_CODE 0x02
#define M3_WEAPON_BINARY_CODE 0x03
#define AWP_WEAPON_BINARY_CODE 0x04

#define KNIFE_WEAPON_BINARY_CODE 0x00
#define PRIMARY_WEAPON_BINARY_CODE 0x01
#define SECONDARY_WEAPON_BINARY_CODE 0x02
#define BOMB_WEAPON_BINARY_CODE 0x03

class WeaponParser {
public:
    uint8_t getWeaponToByte(const GunType& gun);

    WeaponType getWeaponType(const GunType& gun);

    GunType getWeaponFromByte(const uint8_t& weaponCode);

    WeaponType getWeaponTypeFromByte(const uint8_t& weaponCode);

    uint8_t getWeaponTypeToByte(const WeaponType& weaponType);
};

#endif
