#ifndef GUN_H
#define GUN_H

#include <memory>

#include "server/weapon.h"

class Gun: public Weapon {
protected:
    uint16_t ammo;
    Gun(WeaponType type, uint16_t initial_ammo);

public:
    static std::unique_ptr<Gun> new_gun(const WeaponType& type);

    void add_ammo(uint16_t ammo_count);

    Gun(const Gun&) = delete;
    Gun& operator=(const Gun&) = delete;

    virtual ~Gun() = default;
};

#endif
