#include "server/weapon.h"

#include "server/game_world.h"

Weapon::Weapon(): making_action(false), time_since_last_action(0.0f) {}

void Weapon::action() {
    making_action = true;
    time_since_last_action = 0.0f;
}

bool Weapon::just_triggered_action() { return making_action && time_since_last_action == 0.0f; }

void Weapon::stop_action() { making_action = false; }

void Weapon::update(const float& delta_t, Player& /*owner*/, GameWorld& /*game*/) {
    if (time_since_last_action > 0.0f)
        time_since_last_action += delta_t;
}
