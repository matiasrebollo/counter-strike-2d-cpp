#include "server/player.h"

#include <cmath>
#include <iostream>

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
    // enviar eventos si disparo, si mato
    // si mato, reconocerlo y aumentar dinero
}

void Player::move_up() { moving_up = !moving_up; }
void Player::move_down() { moving_down = !moving_down; }
void Player::move_left() { moving_left = !moving_left; }
void Player::move_right() { moving_right = !moving_right; }
void Player::rotate(const double& new_orientation) { this->orientation = new_orientation; }
void Player::stop() {
    moving_up = false;
    moving_down = false;
    moving_left = false;
    moving_right = false;
    orientation = 0.0;
}
void Player::make_action() {
    if (Gun* weapon = loadout.equipped_gun())
        making_action ? weapon->stop_action() : weapon->action();
    // si tiene bomba equipada..

    making_action = !making_action;
}
void Player::equip_primary() { loadout.equip_primary(); }
void Player::equip_secondary() { loadout.equip_secondary(); }
void Player::equip_knife() { loadout.equip_knife(); }

void Player::buy_gun(const GunType& gun) {
    loadout.buy_primary_gun(gun);
    // compra exitosa o no: enviar evento al juego para notificar al cliente??
}
void Player::buy_ammo(const bool& for_primary) {
    loadout.buy_ammo(for_primary);
    // compra exitosa o no: enviar evento al juego para notificar al cliente??
}

const PlayerDTO Player::get_dto() const {
    const PlayerDTO dto{name, rect.position, orientation, life, loadout.get_dto()};
    return dto;
}

Player::~Player() {}
