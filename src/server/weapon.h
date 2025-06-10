#ifndef WEAPON_H
#define WEAPON_H

#include <memory>

class GameWorld;  // forward declaration

#include "common/weapon_type.h"

class Weapon {
protected:
    bool making_action;
    float time_since_last_action;

    bool just_triggered_action();

public:
    Weapon();

    virtual void action();
    void stop_action();
    virtual void update(GameWorld& game, const float& delta_t) = 0;

    Weapon(const Weapon&) = delete;
    Weapon& operator=(const Weapon&) = delete;

    virtual ~Weapon() = default;
};

#endif
