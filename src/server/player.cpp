#include "server/player.h"

#include <iostream>

#include "server/game_world.h"

Player::Player(const std::string& name, Vector2D& position):
        Collidable(position, PLAYER_WIDTH, PLAYER_HEIGHT),
        name(name),
        moving_up(false),
        moving_down(false),
        moving_left(false),
        moving_right(false),
        life(PLAYER_INITIAL_LIFE),
        loadout() {}

bool Player::is_alive() const { return this->life > 0; }

void Player::update(GameWorld& game) {
    if (moving_up) {
        step(Vector2D(0, -1), game);
    }
    if (moving_down) {
        step(Vector2D(0, 1), game);
    }
    if (moving_left) {
        step(Vector2D(-1, 0), game);
    }
    if (moving_right) {
        step(Vector2D(1, 0), game);
    }
    // si esta disparando, ... update de weapons necesario
    // enviar eventos si disparo, si mato
    // si mato, reconocerlo y aumentar dinero
}

void Player::step(const Vector2D& step_dir, GameWorld& game) {
    std::cout << "Estaba en " << this->rect.position.x << ", " << this->rect.position.y
              << std::endl;
    game.make_step_player(*this, step_dir);
    std::cout << "me fui a  " << this->rect.position.x << ", " << this->rect.position.y
              << std::endl;
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
    Weapon* weapon = loadout.equipped_weapon();
    if (weapon)
        weapon->action();
    // necesario saber cuándo se da la acción para manejar el "mantener el click apretado"
}
void Player::equip_primary() { loadout.equip_primary(); }
void Player::equip_secondary() { loadout.equip_secondary(); }
void Player::equip_knife() { loadout.equip_knife(); }

void Player::buy_gun(const GunType& gun) {
    loadout.buy_primary_gun(gun);
    // compra exitosa o no: enviar evento al juego para notificar al cliente??
}
void Player::buy_ammo(const uint16_t& ammo, const bool& for_primary) {
    loadout.buy_ammo(ammo, for_primary);
    // compra exitosa o no: enviar evento al juego para notificar al cliente??
}

const PlayerDTO Player::get_dto() const {
    const PlayerDTO dto{name, rect.position, orientation, life, loadout.get_dto()};
    return dto;
}

/*
void Player::shoot(const CS2DGame& game) const {
    const Vector2D origin(rect.position.x + rect.width / 2,
                          rect.position.y + rect.height / 2);
    Shot shot(origin, orientation);

    const Collidable* hit = shot.shoot(game);

    if (hit != nullptr) {
        // que hit reciba daño de shot, reemplazar prints
        rect h = hit->get_rect();
        std::cout << "¡Impacto! Disparo acertó a objeto en (" << h.position.x << ", "
                  << h.position.y << ")\n";
    } else {
        std::cout << "Disparo fallido. No impactó ningún objeto.\n";
    }
}*/

Player::~Player() {}
