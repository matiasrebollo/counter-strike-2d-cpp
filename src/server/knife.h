#ifndef KNIFE_H
#define KNIFE_H

#include <memory>

#include "server/weapon.h"

class Knife: public Weapon {
    void stab();

public:
    Knife();

    void action() override;

    Knife(const Knife&) = delete;
    Knife& operator=(const Knife&) = delete;

    virtual ~Knife() = default;
};

#endif
