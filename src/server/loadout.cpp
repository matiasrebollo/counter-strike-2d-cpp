#include "server/loadout.h"

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

Loadout::Loadout():
        money(INITIAL_MONEY),
        knife(),
        primary_gun(nullptr),
        secondary_gun(Gun::new_gun(GLOCK)),
        bomb(nullptr),
        equipped(SECONDARY) {}

int Loadout::get_money() const { return money; }

bool Loadout::has_primary_gun() const { return primary_gun != nullptr; }
bool Loadout::has_bomb() const { return bomb != nullptr; }

GunType Loadout::primary_gun_type() const {
    if (primary_gun == nullptr)
        return NONE;
    return primary_gun->get_type();
}
GunType Loadout::secondary_gun_type() const { return secondary_gun->get_type(); }

void Loadout::decrease_money_by(const int& ammount_of_money) {
    if (ammount_of_money <= 0)
        return;

    money = std::max(0, money - ammount_of_money);
}

void Loadout::add_money(const int& ammount_of_money) { this->money += ammount_of_money; }

void Loadout::receive_bomb(std::shared_ptr<Bomb> bomb) { this->bomb = bomb; }

void Loadout::leave_bomb() { this->bomb = nullptr; }

std::unique_ptr<Gun> Loadout::take_primary_gun() { return std::move(this->primary_gun); }

std::unique_ptr<Gun> Loadout::new_primary_gun(std::unique_ptr<Gun> gun) {
    std::unique_ptr<Gun> prev = std::move(this->primary_gun);
    this->primary_gun = std::move(gun);
    return prev;
}

std::unique_ptr<Gun> Loadout::new_secondary_gun(std::unique_ptr<Gun> gun) {
    std::unique_ptr<Gun> prev = std::move(this->secondary_gun);
    this->secondary_gun = std::move(gun);
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
void Loadout::equip_bomb() {
    if (bomb)
        this->equipped = BOMB;
}

WeaponType Loadout::get_equipped() const { return equipped; }

Weapon* Loadout::equipped_weapon() {
    switch (equipped) {
        case PRIMARY:
            return primary_gun ? primary_gun.get() : nullptr;
        case SECONDARY:
            return secondary_gun.get();
        case KNIFE:
            return &knife;
        case BOMB:
            return bomb ? bomb.get() : nullptr;
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
                      equipped,
                      bomb != nullptr};
}

void Loadout::reset(const bool& on_death) {
    if (!on_death)
        money = INITIAL_MONEY;
    primary_gun = nullptr;
    secondary_gun = Gun::new_gun(GLOCK);
    bomb = nullptr;
    equipped = SECONDARY;
}

Loadout::~Loadout() {}
