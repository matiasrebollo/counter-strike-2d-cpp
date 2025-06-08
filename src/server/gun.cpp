#include "server/gun.h"

Gun::Gun(const GunType& gun_type): is_trigger_pressed(false), type(gun_type) {
    switch (type) {
        case GunType::GLOCK:
            ammo = GLOCK_INITIAL_AMMO;
            break;
        case GunType::AK47:
            ammo = AK47_INITIAL_AMMO;
            break;
        case GunType::AWP:
            ammo = AWP_INITIAL_AMMO;
            break;
        case GunType::M3:
            ammo = M3_INITIAL_AMMO;
            break;
        case GunType::NONE:
            ammo = 0;
            break;
        default:
            throw std::invalid_argument("not a gun type");
    }
}

/*std::unique_ptr<Gun> Gun::new_gun(const GunType& type) {
    switch (type) {
        case GunType::GLOCK:
            return std::make_unique<Glock>();
        case GunType::AK47:
            return std::make_unique<AK47>();
        case GunType::AWP:
            return std::make_unique<AWP>();
        case GunType::M3:
            return std::make_unique<M3>();
        default:
            throw std::invalid_argument("not a gun type");
    }
}*/

void Gun::add_ammo(uint16_t ammo_count) { ammo += ammo_count; }

void Gun::action() {
    is_trigger_pressed = !is_trigger_pressed;
    // shoot() si no estaba presionado??
}

void Gun::shoot() {}

uint16_t Gun::get_ammo() const { return ammo; }

GunType Gun::get_type() const { return type; }
