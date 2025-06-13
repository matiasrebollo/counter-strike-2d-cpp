#include "server/loadout.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

Loadout::Loadout():
        money(INITIAL_MONEY),
        knife(),
        primary_gun(nullptr),
        secondary_gun(Gun::new_gun(GLOCK)),
        equipped(SECONDARY) {}

int Loadout::get_money() const { return money; }

bool Loadout::has_primary_gun() const { return primary_gun != nullptr; }

GunType Loadout::primary_gun_type() const {
    if (primary_gun == nullptr)
        return NONE;
    return primary_gun->get_type();
}
GunType Loadout::secondary_gun_type() const { return secondary_gun->get_type(); }

void Loadout::decrease_money_by(const uint16_t& ammount_of_money) {
    this->money -= ammount_of_money;
}

std::unique_ptr<Gun> Loadout::new_primary_gun(std::unique_ptr<Gun> gun) {
    std::unique_ptr<Gun> prev = std::move(this->primary_gun);
    this->primary_gun = std::move(gun);
    return prev;
}

void Loadout::add_ammo_to_primary(const int& ammo_count) { primary_gun->add_ammo(ammo_count); }
void Loadout::add_ammo_to_secondary(const int& ammo_count) { secondary_gun->add_ammo(ammo_count); }

void Loadout::equip_primary() {
    if (primary_gun)
        this->equipped = PRIMARY;
}
void Loadout::equip_secondary() { this->equipped = SECONDARY; }
void Loadout::equip_knife() { this->equipped = KNIFE; }
Weapon* Loadout::equipped_weapon() {
    switch (equipped) {
        case PRIMARY:
            return primary_gun ? primary_gun.get() : nullptr;
        case SECONDARY:
            return secondary_gun.get();
        case KNIFE:
            return &knife;
        default:
            return nullptr;
    }
}

const LoadoutDTO Loadout::get_dto() const {
    return LoadoutDTO{money,
                      (primary_gun != nullptr) ? primary_gun->get_type() : GunType::NONE,
                      (primary_gun != nullptr) ? primary_gun->get_ammo() : static_cast<uint16_t>(0),
                      GLOCK,
                      secondary_gun->get_ammo(),
                      equipped};
    // agregar bomba
}

Loadout::~Loadout() {}
