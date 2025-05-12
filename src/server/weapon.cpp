#include "server/weapon.h"

Weapon::Weapon(WeaponType type): type(type) {}

WeaponType Weapon::get_type() const { return type; }
