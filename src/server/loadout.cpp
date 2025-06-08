#include "server/loadout.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

Loadout::Loadout():
        money(INITIAL_MONEY), primary_gun(nullptr), secondary_gun(GLOCK), equipped(KNIFE) {}

uint16_t Loadout::ammo_per_clip_for(const GunType& type) {
    // está hardcodeado. inicializar un map a partir del archivo de configuracion al inicializar el
    // Loadout
    switch (type) {
        case GunType::GLOCK:
            return 50;
        case GunType::AK47:
            return 20;
        case GunType::M3:
            return 80;
        case GunType::AWP:
            return 4;
        default:
            throw std::invalid_argument(
                    "Weapon::ammo_per_clip_for: tipo de arma no válido para la compra de balas");
    }
}

uint16_t Loadout::price_for(const GunType& type) {
    // está hardcodeado. inicializar un map a partir del archivo de configuracion al inicializar el
    // Loadout
    switch (type) {
        case GunType::AK47:
            return 2500;
        case GunType::M3:
            return 1700;
        case GunType::AWP:
            return 4750;
        default:
            throw std::invalid_argument(
                    "Loadout::price_for: tipo de arma no válido para la compra");
    }
}

void Loadout::decrease_money_by(const uint16_t& ammount_of_money) {
    this->money -= ammount_of_money;
}

std::unique_ptr<Gun> Loadout::new_primary_gun(std::unique_ptr<Gun> gun) {
    std::unique_ptr<Gun> prev = std::move(this->primary_gun);
    this->primary_gun = std::move(gun);
    return prev;
}


bool Loadout::can_buy_gun(const GunType& gun_type) const {
    return this->money >= price_for(gun_type);
}

std::unique_ptr<Gun> Loadout::buy_primary_gun(const GunType& gun_type) {
    if (!can_buy_gun(gun_type)) {
        return nullptr;
    }
    decrease_money_by(price_for(gun_type));
    auto bought_gun = std::make_unique<Gun>(gun_type);
    return new_primary_gun(std::move(bought_gun));
}

bool Loadout::buy_ammo(const bool& for_primary) {
    if (money < CLIP_PRICE || (!primary_gun && for_primary))
        return false;

    decrease_money_by(CLIP_PRICE);
    if (for_primary)
        primary_gun->add_ammo(ammo_per_clip_for(primary_gun->get_type()));

    if (!for_primary)
        secondary_gun.add_ammo(ammo_per_clip_for(secondary_gun.get_type()));

    return true;
}

void Loadout::equip_primary() { this->equipped = PRIMARY; }
void Loadout::equip_secondary() { this->equipped = SECONDARY; }
void Loadout::equip_knife() { this->equipped = KNIFE; }
Weapon* Loadout::equipped_weapon() {
    switch (equipped) {
        case PRIMARY:
            return primary_gun ? primary_gun.get() : nullptr;
        case SECONDARY:
            return &secondary_gun;
        case KNIFE:
            return &knife;
        default:
            return nullptr;
    }
}

const LoadoutDTO Loadout::get_dto() const {
    std::string p_str;
    if (primary_gun != nullptr) {
        switch (primary_gun->get_type()) {
            case GunType::AK47:
                p_str = "AK47";
                break;
            case GunType::M3:
                p_str = "M3";
                break;
            case GunType::AWP:
                p_str = "AWP";
                break;
            default:
                p_str = "Unknown";
                break;
        }
    } else {
        p_str = "NONE";
    }
    int p_ammo = 0;
    if (primary_gun != nullptr) {
        p_ammo = primary_gun->get_ammo();
    }
    std::cout << "Money: " << money << ". Arma primaria: " << p_str << " (" << p_ammo
              << "). Arma secundaria: GLOCK"
              << " (" << secondary_gun.get_ammo() << ")" << std::endl;
    return LoadoutDTO{money,
                      (primary_gun != nullptr) ? primary_gun->get_type() : GunType::NONE,
                      (primary_gun != nullptr) ? primary_gun->get_ammo() : static_cast<uint16_t>(0),
                      GLOCK,
                      secondary_gun.get_ammo(),
                      equipped};
    // agregar bomba
    // agregar disparos ??
}

Loadout::~Loadout() {}
