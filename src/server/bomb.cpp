#include "server/bomb.h"

#include <iostream>

#include "server/game_world.h"

Bomb::Bomb():
        status(NOT_PLANTED),
        just_been_planted(false),
        time_since_planted(0.0f),
        plantation(std::nullopt) {}

BombStatus Bomb::get_status() { return status; }

std::optional<Rect>& Bomb::get_plantation() { return plantation; }
std::optional<Vector2D<int>> Bomb::get_plantation_position() {
    if (!plantation)
        return std::nullopt;
    return plantation->position;
}

bool Bomb::just_planted() { return just_been_planted; }
int Bomb::detonation_time() { return DETONATION_TIME; }

void Bomb::action() {
    if (status == DEFUSED) {
        return;
    }
    Weapon::action();
}
void Bomb::stop_action() {
    if (status == DEFUSED) {
        return;
    }
    Weapon::stop_action();
}

void Bomb::update(const float& delta_t, Player& owner, GameWorld& game) {
    if (status == DEFUSED) {
        return;
    }
    Weapon::update(delta_t, owner, game);
    if (making_action && time_since_last_action >= PLANTATION_TIME) {
        just_been_planted = true;
        status = PLANTED;
        plantation.emplace(owner.rect.position, BOMB_THICKNESS, BOMB_THICKNESS);
        owner.leave_bomb();
        time_since_last_action = 0.0f;
        making_action = false;
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
        defuse();
    }
}

void Bomb::restart() {
    status = NOT_PLANTED;
    time_since_planted = 0.0f;
    plantation = std::nullopt;
}

void Bomb::defuse() {
    time_since_last_action = 0.0f;
    making_action = false;
    status = DEFUSED;
}
