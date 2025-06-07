#include "server/gun.h"

Gun::Gun(int initial_ammo): ammo(initial_ammo), is_trigger_pressed(false) {}

std::unique_ptr<Gun> Gun::new_gun(const GunType& type) {
    switch (type) {
        case GunType::GLOCK:
            return std::make_unique<Glock>();
        /*case GunType::AK47:
            return std::make_unique<AK47>();
        case GunType::AWP:
            return std::make_unique<AWP>();
        case GunType::M3:
            return std::make_unique<M3>();*/
        default:
            throw std::invalid_argument("Weapon type is not a gun type");
    }
}

void Gun::add_ammo(uint16_t ammo_count) { ammo += ammo_count; }

void Gun::action() {
    is_trigger_pressed = !is_trigger_pressed;
    // shoot() si no estaba presionado??
}

Glock::Glock(): Gun(GLOCK_INITIAL_AMMO) {}

void Glock::shoot() {}

GunType Glock::type() const { return GLOCK; }
