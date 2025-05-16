#include "weaponParser.h"

#include <unordered_map>

#include "weapon.h"

uint8_t WeaponParser::getWeaponToByte(const Weapon& weapon) {
    std::unordered_map<Weapon, uint8_t> map = {{Weapon::NONE, NONE_WEAPON_BINARY_CODE},
                                               {Weapon::GLOCK, GLOCK_WEAPON_BINARY_CODE},
                                               {Weapon::AK47, AK47_WEAPON_BINARY_CODE},
                                               {Weapon::M3, M3_WEAPON_BINARY_CODE},
                                               {Weapon::AWP, AWP_WEAPON_BINARY_CODE}};

    auto it = map.find(weapon);
    return it != map.end() ? it->second : UNKNOWON_WEAPON_BINARY_CODE;
}

WeaponType WeaponParser::getWeaponType(const Weapon& weapon) {
    if (weapon == Weapon::GLOCK) {
        return WeaponType::SECONDARY;
    } else {
        return WeaponType::PRIMARY;
    }
}

Weapon WeaponParser::getWeaponFromByte(const uint8_t& weaponCode) {
    std::unordered_map<uint8_t, Weapon> map = {{NONE_WEAPON_BINARY_CODE, Weapon::NONE},
                                               {GLOCK_WEAPON_BINARY_CODE, Weapon::GLOCK},
                                               {AK47_WEAPON_BINARY_CODE, Weapon::AK47},
                                               {M3_WEAPON_BINARY_CODE, Weapon::M3},
                                               {AWP_WEAPON_BINARY_CODE, Weapon::AWP}};

    auto it = map.find(weaponCode);
    return it != map.end() ? it->second : UNKNOWN_WEAPON;
}

WeaponType WeaponParser::getWeaponTypeFromByte(const uint8_t& weaponCode) {
    const std::unordered_map<uint8_t, WeaponType> map = {
            {PRIMARY_WEAPON_BINARY_CODE, WeaponType::PRIMARY},
            {SECONDARY_WEAPON_BINARY_CODE, WeaponType::SECONDARY}};

    auto it = map.find(weaponCode);
    return it != map.end() ? it->second : UNKNOWN_WEAPON_TYPE;
}

uint8_t WeaponParser::getWeaponTypeToByte(const WeaponType& weaponType) {
    const std::unordered_map<WeaponType, uint8_t> map = {
            {WeaponType::PRIMARY, PRIMARY_WEAPON_BINARY_CODE},
            {WeaponType::SECONDARY, SECONDARY_WEAPON_BINARY_CODE}};

    auto it = map.find(weaponType);
    return it != map.end() ? it->second : UNKNOWN_WEAPON_TYPE;
}
