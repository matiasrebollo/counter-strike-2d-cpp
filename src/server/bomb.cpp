#include "server/bomb.h"

#include <iostream>

#include "server/game_world.h"

Bomb::Bomb():
        status(NOT_PLANTED),
        just_been_planted(false),
        time_since_planted(0.0f),
        plantation(std::nullopt),
        explosion_radius(Settings::getInstance().get_explosion_radius()),
        explosion_damage(Settings::getInstance().get_bomb_damage()) {}

BombStatus Bomb::get_status() { return status; }

std::optional<Rect>& Bomb::get_plantation() { return plantation; }
std::optional<Vector2D<int>> Bomb::get_plantation_position() {
    if (!plantation)
        return std::nullopt;
    return plantation->position;
}

int Bomb::get_explosion_radius() const { return explosion_radius; }

bool Bomb::just_planted() { return just_been_planted; }
int Bomb::detonation_time() { return Settings::getInstance().get_detonation_time(); }

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
    if (making_action && time_since_last_action >= Settings::getInstance().get_plantation_time()) {
        just_been_planted = true;
        status = PLANTED;
        Vector2D<int> plantation_posicion(owner.rect.position.x + owner.rect.width / 2,
                                          owner.rect.position.y + owner.rect.height / 2);
        plantation.emplace(plantation_posicion, BOMB_THICKNESS, BOMB_THICKNESS);
        owner.leave_bomb();
        time_since_last_action = 0.0f;
        making_action = false;
    }
}

void Bomb::update_planted(const float& delta_t) {
    just_been_planted = false;
    time_since_planted += delta_t;
    if (just_triggered_action) {
        just_triggered_action = false;
    } else {
        time_since_last_action += delta_t;
    }
    if (time_since_planted >= Settings::getInstance().get_detonation_time()) {
        status = EXPLODED;
        return;
    }
    if (making_action && time_since_last_action >= Settings::getInstance().get_defuse_time()) {
        defuse();
    }
}

void Bomb::make_damage_to(Player& victim, const float& distance_to_victim, GameWorld& game) {
    float damage_ratio = 1.0f - (distance_to_victim / explosion_radius);
    int damage = static_cast<int>(explosion_damage * damage_ratio);
    victim.receive_damage(damage, game);
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
