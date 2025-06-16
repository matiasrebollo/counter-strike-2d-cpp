#ifndef WEAPON_H
#define WEAPON_H

#include <memory>

class GameWorld;  // forward declaration
class Player;     // forward declaration

#include "common/weapon_type.h"

class Weapon {
protected:
    bool making_action;
    bool just_triggered_action;
    float time_since_last_action;

    Weapon();

public:
    virtual void action();
    void stop_action();
    virtual void update(const float& delta_t, Player& owner, GameWorld& game);

    Weapon(const Weapon&) = delete;
    Weapon& operator=(const Weapon&) = delete;

    virtual ~Weapon() = default;
};

#endif
