#ifndef WEAPON_H
#define WEAPON_H

#include <memory>

#include "server/weapon_type.h"

class Weapon {
protected:
    WeaponType type;

    explicit Weapon(WeaponType type);

public:
    virtual WeaponType get_type() const;

    // virtual int damage() const = 0;

    virtual ~Weapon() = default;

    Weapon(const Weapon&) = delete;
    Weapon& operator=(const Weapon&) = delete;

    Weapon(Weapon&&) noexcept = default;
    Weapon& operator=(Weapon&&) noexcept = default;
};

#endif
