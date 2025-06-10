#include "server/player.h"

#include <algorithm>

#include "server/game_world.h"

Player::Player(const std::string& name, Vector2D<int>& position):
        Collidable(position, PLAYER_WIDTH, PLAYER_HEIGHT),
        name(name),
        moving_up(false),
        moving_down(false),
        moving_left(false),
        moving_right(false),
        making_action(false),
        life(PLAYER_INITIAL_LIFE),
        loadout() {}

float Player::get_orientation() const { return orientation; }

bool Player::is_alive() const { return this->life > 0; }

void Player::update(GameWorld& game, const float& delta_t) {
    int stepped = static_cast<int>(std::round(delta_t * PLAYER_SPEED));
    if (moving_up) {
        game.make_step_player(*this, Vector2D<int>(0, -stepped));
    }
    if (moving_down) {
        game.make_step_player(*this, Vector2D<int>(0, stepped));
    }
    if (moving_left) {
        game.make_step_player(*this, Vector2D<int>(-stepped, 0));
    }
    if (moving_right) {
        game.make_step_player(*this, Vector2D<int>(stepped, 0));
    }
    if (Gun* weapon = loadout.equipped_gun())
        weapon->update(delta_t, *this, game);
    // si mato, reconocerlo y aumentar dinero
}

void Player::move_up() { moving_up = true; }
void Player::move_down() { moving_down = true; }
void Player::move_left() { moving_left = true; }
void Player::move_right() { moving_right = true; }
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
    orientation = 0.0;
}
void Player::make_action() {
    if (Gun* weapon = loadout.equipped_gun())
        weapon->action();
    // si tiene bomba equipada..

    making_action = true;
}
void Player::stop_making_action() {
    if (Gun* weapon = loadout.equipped_gun())
        weapon->stop_action();
    // si tiene bomba equipada..

    making_action = false;
}

void Player::receive_damage(const int& damage) { life = std::max(life - damage, 0); }

void Player::equip_primary() { loadout.equip_primary(); }
void Player::equip_secondary() { loadout.equip_secondary(); }
void Player::equip_knife() { loadout.equip_knife(); }

void Player::buy_gun(const GunType& gun) { loadout.buy_primary_gun(gun); }
void Player::buy_ammo(const bool& for_primary) { loadout.buy_ammo(for_primary); }

const PlayerDTO Player::get_dto() const {
    const PlayerDTO dto{name, rect.position, orientation, life, loadout.get_dto()};
    return dto;
}


Player::~Player() {}
