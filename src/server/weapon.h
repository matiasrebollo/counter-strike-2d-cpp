#ifndef WEAPON_H
#define WEAPON_H

#include <memory>

#include "common/weapon_type.h"

class Weapon {
protected:
    Weapon();

public:
    // virtual int damage() const = 0;

    virtual ~Weapon() = default;

    Weapon(const Weapon&) = delete;
    Weapon& operator=(const Weapon&) = delete;
};

#endif
