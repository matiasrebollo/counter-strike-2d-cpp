#ifndef LOADOUT_H
#define LOADOUT_H

#include <cstdint>
#include <memory>
#include <optional>

#include "common/loadout_dto.h"
#include "common/settings.h"
#include "server/gun.h"
#include "server/knife.h"

class Loadout {
private:
    uint16_t money;
    Knife knife;
    std::unique_ptr<Gun> primary_gun;
    std::unique_ptr<Gun> secondary_gun;
    WeaponType equipped;


public:
    Loadout();

    int get_money() const;
    bool has_primary_gun() const;
    GunType primary_gun_type() const;
    GunType secondary_gun_type() const;
    void decrease_money_by(const int& amount_of_money);
    void add_money(const int& ammount_of_money);
    // devuelve el arma poseída anteriormente.
    std::unique_ptr<Gun> new_primary_gun(std::unique_ptr<Gun> gun);
    void add_ammo_to_primary(const int& ammo_count);
    void add_ammo_to_secondary(const int& ammo_count);
    void equip_primary();
    void equip_secondary();
    void equip_knife();
    Weapon* equipped_weapon();
    const LoadoutDTO get_dto() const;

    Loadout(const Loadout&) = delete;
    Loadout& operator=(const Loadout&) = delete;

    ~Loadout();
};

#endif
