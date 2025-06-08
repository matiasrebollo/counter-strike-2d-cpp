#ifndef WEAPON_H
#define WEAPON_H

#include <memory>

#include "common/weapon_type.h"

class Weapon {
public:
    Weapon() = default;

    virtual void action() = 0;
    // update

    Weapon(const Weapon&) = delete;
    Weapon& operator=(const Weapon&) = delete;

    virtual ~Weapon() = default;
};

#endif
