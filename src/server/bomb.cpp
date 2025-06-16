#include "server/bomb.h"

#include "server/game_world.h"

Bomb::Bomb(): status(NOT_PLANTED), just_been_planted(false), time_since_planted(0.0f) {}

BombStatus Bomb::get_status() { return status; }

bool Bomb::just_planted() { return just_been_planted; }
int Bomb::detonation_time() { return DETONATION_TIME; }

void Bomb::update(const float& delta_t, Player& owner, GameWorld& game) {
    Weapon::update(delta_t, owner, game);
    if (making_action && time_since_last_action >= PLANTATION_TIME) {
        just_been_planted = true;
        status = PLANTED;
        game.plant_bomb(owner);
    }
}

void Bomb::update_planted(const float& delta_t) {
    just_been_planted = false;
    if (just_triggered_action) {
        just_triggered_action = false;
        return;
    }
    time_since_last_action += delta_t;
    time_since_planted += delta_t;
    if (time_since_planted >= DETONATION_TIME) {
        status = EXPLODED;
        return;
    }
    if (making_action && time_since_last_action >= DEFUSE_TIME) {
        status = DEFUSED;
    }
}


void Bomb::restart() { status = NOT_PLANTED; }
