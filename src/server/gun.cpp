#include "server/gun.h"

#include "server/ak47.h"
#include "server/awp.h"
#include "server/glock.h"
#include "server/m3.h"

std::unique_ptr<Gun> Gun::new_gun(const WeaponType& type) {
    switch (type) {
        case WeaponType::AK47:
            return std::make_unique<Glock>();
        case WeaponType::AK47:
            return std::make_unique<AK47>();
        case WeaponType::AWP:
            return std::make_unique<AWP>();
        case WeaponType::M3:
            return std::make_unique<M3>();
        default:
            throw std::invalid_argument("Weapon type is not a gun type");
    }
}

Gun::Gun(const GunType& type, uint16_t initial_ammo): type(type), ammo(initial_ammo) {}

void Gun::add_ammo(uint16_t ammo_count) { ammo += ammo_count; }
