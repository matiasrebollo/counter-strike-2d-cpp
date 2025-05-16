#include "weapon_parser.h"

#include <unordered_map>

uint8_t WeaponParser::getWeaponToByte(const GunType& weapon) {
    std::unordered_map<GunType, uint8_t> map = {{GunType::NONE, NONE_WEAPON_BINARY_CODE},
                                               {GunType::GLOCK, GLOCK_WEAPON_BINARY_CODE},
                                               {GunType::AK47, AK47_WEAPON_BINARY_CODE},
                                               {GunType::M3, M3_WEAPON_BINARY_CODE},
                                               {GunType::AWP, AWP_WEAPON_BINARY_CODE}};

    auto it = map.find(weapon);
    return it->second;
}

WeaponType WeaponParser::getWeaponType(const GunType& weapon) {
    if (weapon == GunType::GLOCK) {
        return WeaponType::SECONDARY;
    } else {
        return WeaponType::PRIMARY;
    }
}

GunType WeaponParser::getWeaponFromByte(const uint8_t& weaponCode) {
    std::unordered_map<uint8_t, GunType> map = {{NONE_WEAPON_BINARY_CODE, GunType::NONE},
                                               {GLOCK_WEAPON_BINARY_CODE, GunType::GLOCK},
                                               {AK47_WEAPON_BINARY_CODE, GunType::AK47},
                                               {M3_WEAPON_BINARY_CODE, GunType::M3},
                                               {AWP_WEAPON_BINARY_CODE, GunType::AWP}};

    auto it = map.find(weaponCode);
    return it->second;
}

WeaponType WeaponParser::getWeaponTypeFromByte(const uint8_t& weaponCode) {
    const std::unordered_map<uint8_t, WeaponType> map = {
            {PRIMARY_WEAPON_BINARY_CODE, WeaponType::PRIMARY},
            {SECONDARY_WEAPON_BINARY_CODE, WeaponType::SECONDARY}};

    auto it = map.find(weaponCode);
    return it->second;
}

uint8_t WeaponParser::getWeaponTypeToByte(const WeaponType& weaponType) {
    const std::unordered_map<WeaponType, uint8_t> map = {
            {WeaponType::PRIMARY, PRIMARY_WEAPON_BINARY_CODE},
            {WeaponType::SECONDARY, SECONDARY_WEAPON_BINARY_CODE}};

    auto it = map.find(weaponType);
    return it->second;
}
