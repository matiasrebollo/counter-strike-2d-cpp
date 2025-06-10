#include "server/knife.h"

Knife::Knife(): time_since_last_stab(0.0f) {}

bool Knife::can_stab() { return time_since_last_stab >= (KNIFE_ROF / 60.0f); }

void Knife::stab(GameWorld& /*game*/) {}

void Knife::update(GameWorld& game, const float& delta_t) {
    if (just_triggered_action() && can_stab()) {
        stab(game);
        time_since_last_stab = 0.0f;
    } else {
        time_since_last_stab += delta_t;
    }
    Weapon::update(game, delta_t);
}
