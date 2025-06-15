#include "server/player.h"

#include <algorithm>
#include <iostream>

#include "server/game_world.h"

Player::Player(const std::string& name, Vector2D<int>& position):
        Collidable(position, PLAYER_THICKNESS, PLAYER_THICKNESS),
        name(name),
        moving_up(false),
        moving_down(false),
        moving_left(false),
        moving_right(false),
        making_action(false),
        orientation(0.0),
        life(PLAYER_INITIAL_LIFE),
        shot(std::nullopt),
        bonifications(0),
        kills(0),
        deaths(0),
        loadout() {}

float Player::get_orientation() const { return orientation; }

bool Player::is_alive() const { return this->life > 0; }

void Player::update(GameWorld& game, const float& delta_t) {
    shot = std::nullopt;
    int delta_it = static_cast<int>(std::round(delta_t * FPS_SERVER));
    int stepped = delta_it * PLAYER_SPEED;
    Vector2D<int> step(0, 0);
    if (moving_up) {
        step = step + Vector2D<int>(0, -stepped);
    }
    if (moving_down) {
        step = step + Vector2D<int>(0, stepped);
    }
    if (moving_left) {
        step = step + Vector2D<int>(-stepped, 0);
    }
    if (moving_right) {
        step = step + Vector2D<int>(stepped, 0);
    }
    if (step.x != 0 && step.y != 0) {
        step.x = static_cast<int>(step.x / std::sqrt(2));
        step.y = static_cast<int>(step.y / std::sqrt(2));
    }
    game.make_step_player(*this, step);
    if (Weapon* weapon = loadout.equipped_weapon())
        weapon->update(delta_t, *this, game);
    // si mato, reconocerlo y aumentar dinero
}

void Player::move_up() { moving_up = true; }
void Player::move_down() { moving_down = true; }
void Player::move_left() { moving_left = true; }
void Player::move_right() { moving_right = true; }

bool Player::collides_with(const Collidable& other_collidable) const {
    if (!is_alive())
        return false;
    return rect.intersects_with(other_collidable.rect);
}


void Player::stop_moving_up() { moving_up = false; }
void Player::stop_moving_down() { moving_down = false; }
void Player::stop_moving_left() { moving_left = false; }
void Player::stop_moving_right() { moving_right = false; }
void Player::rotate(const double& new_orientation) { this->orientation = new_orientation; }
void Player::restart() {
    life = PLAYER_INITIAL_LIFE;
    moving_up = false;
    moving_down = false;
    moving_left = false;
    moving_right = false;
    making_action = false;
    orientation = 0.0;
}
void Player::make_action() {
    if (Weapon* weapon = loadout.equipped_weapon())
        weapon->action();
    making_action = true;
}
void Player::stop_making_action() {
    if (Weapon* weapon = loadout.equipped_weapon())
        weapon->stop_action();
    making_action = false;
}

void Player::shoot(const Shot& a_shot) { shot = ShotDTO{a_shot.distance}; }

void Player::receive_damage(const int& damage) {
    life = std::max(life - damage, 0);
    if (life == 0)
        deaths += 1;
}
void Player::count_kill(const int& money_bonification) {
    kills += 1;
    bonifications += money_bonification;
    loadout.add_money(money_bonification);
}

void Player::equip_primary() { loadout.equip_primary(); }
void Player::equip_secondary() { loadout.equip_secondary(); }
void Player::equip_knife() { loadout.equip_knife(); }

Loadout& Player::get_loadout() { return loadout; }

const PlayerDTO Player::get_dto() const {
    return PlayerDTO{name,  rect.position, orientation,      life, shot, bonifications,
                     kills, deaths,        loadout.get_dto()};
}


Player::~Player() {}
