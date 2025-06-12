#include "server/weapon.h"

#include <iostream>

#include "server/game_world.h"

Weapon::Weapon():
        making_action(false), just_triggered_action(false), time_since_last_action(0.0f) {}

void Weapon::action() {
    making_action = true;
    just_triggered_action = true;
    time_since_last_action = 0.0f;
}

void Weapon::stop_action() { making_action = false; }

void Weapon::update(const float& delta_t, Player& /*owner*/, GameWorld& /*game*/) {
    if (just_triggered_action) {
        just_triggered_action = false;
        return;
    }
    time_since_last_action += delta_t;
}
