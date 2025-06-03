#include "server/loadout_manager.h"

#include <memory>
#include <stdexcept>
#include <utility>

LoadoutManager::LoadoutManager():
        money(INITIAL_MONEY), primary_gun(nullptr), secondary_gun(), equipped(KNIFE) {}

uint16_t LoadoutManager::ammo_price_for(const GunType& type) {
    // está hardcodeado. inicializar un map a partir del archivo de configuracion al inicializar el
    // LoadoutManager
    switch (type) {
        case GunType::GLOCK:
            return 10;
        case GunType::AK47:
            return 30;
        case GunType::M3:
            return 20;
        case GunType::AWP:
            return 50;
        default:
            throw std::invalid_argument(
                    "Weapon::ammo_price_for: tipo de arma no válido para la compra de balas");
    }
}

uint16_t LoadoutManager::price_for(const GunType& type) {
    // está hardcodeado. inicializar un map a partir del archivo de configuracion al inicializar el
    // LoadoutManager
    switch (type) {
        case GunType::AK47:
            return 1500;
        case GunType::M3:
            return 1000;
        case GunType::AWP:
            return 1200;
        default:
            throw std::invalid_argument("Weapon::price_for: tipo de arma no válido para la compra");
    }
}

void LoadoutManager::decrease_money_by(const uint16_t& ammount_of_money) {
    this->money -= ammount_of_money;
}

const std::unique_ptr<Gun> LoadoutManager::new_primary_gun(std::unique_ptr<Gun> gun) {
    std::unique_ptr<Gun> prev = std::move(this->primary_gun);
    this->primary_gun = std::move(gun);
    return prev;
}

bool LoadoutManager::can_buy_gun(const GunType& gun_type) const {
    return this->money >= price_for(gun_type);
}

bool LoadoutManager::can_buy_ammo(const uint16_t& ammo_count, bool for_primary) const {
    uint16_t price = 0;
    if (for_primary) {
        if (not primary_gun)
            return false;
        price = ammo_price_for(this->primary_gun->type());
    }
    if (not for_primary)
        price = ammo_price_for(this->secondary_gun.type());

    return this->money >= price * ammo_count;
}

const std::unique_ptr<Gun> LoadoutManager::buy_primary_gun(const GunType& gun_type) {
    if (!can_buy_gun(gun_type)) {
        return nullptr;
    }
    decrease_money_by(price_for(gun_type));
    return new_primary_gun(Gun::new_gun(gun_type));
}

bool LoadoutManager::buy_ammo(const uint16_t& ammo_count, bool for_primary) {
    if (not can_buy_ammo(ammo_count, for_primary))
        return false;

    if (for_primary)
        this->primary_gun->add_ammo(ammo_count);

    if (not for_primary)
        this->secondary_gun.add_ammo(ammo_count);

    return true;
}

void LoadoutManager::equip_primary() { this->equipped = PRIMARY; }
void LoadoutManager::equip_secondary() { this->equipped = SECONDARY; }
void LoadoutManager::equip_knife() { this->equipped = KNIFE; }

LoadoutManager::~LoadoutManager() {}
