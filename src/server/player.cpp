#include "server/player.h"

#include <iostream>

#include "server/cs2d_game.h"

Player::Player(Vector2D& position, double& orientation, ClientSender& sender):
        Collidable(position, PLAYER_WIDTH, PLAYER_HEIGHT),
        moving_up(false),
        moving_down(false),
        moving_left(false),
        moving_right(false),
        orientation(orientation),
        life(PLAYER_INITIAL_LIFE),
        sender(sender) {}

float Player::get_orientation() const { return orientation; }
uint16_t Player::get_life() const { return life; }

void Player::update(CS2DGame& game) {
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
    // si esta disparando, ...
}

void Player::step(const Vector2D& step_dir, CS2DGame& game) {
    Hitbox old_hitbox = Hitbox(this->hitbox);
    std::cout << "posicion anterior: x: " << old_hitbox.position.x
              << ", y: " << old_hitbox.position.y << std::endl;

    this->hitbox.position = hitbox.position + step_dir * PLAYER_SPEED;
    std::cout << "me quiero mover a: x: " << this->hitbox.position.x
              << ", y: " << this->hitbox.position.y << std::endl;
    const bool collision = game.is_player_not_in_valid_position(*this);
    if (collision) {
        std::cout << "colision!" << std::endl;
        // ver de "avanzar lo mas posible" en vez de calcelar el movimiento???
        this->hitbox = old_hitbox;
        return;
    }
    std::cout << "posicion final: x: " << this->hitbox.position.x
              << ", y: " << this->hitbox.position.y << std::endl;
    std::cout << "\n";
}

void Player::move_up() { moving_up = !moving_up; }
void Player::move_down() { moving_down = !moving_down; }
void Player::move_left() { moving_left = !moving_left; }
void Player::move_right() { moving_right = !moving_right; }
void Player::rotate(const double& new_orientation) { this->orientation = new_orientation; }

void Player::send_map(const GameMap& map) { sender.send_map(map); }
void Player::send_snapshot(const Snapshot& snapshot) { sender.push(snapshot); }

/*
void Player::shoot(const CS2DGame& game) const {
    const Vector2D origin(hitbox.position.x + hitbox.width / 2,
                          hitbox.position.y + hitbox.height / 2);
    Shot shot(origin, orientation);

    const Collidable* hit = shot.shoot(game);

    if (hit != nullptr) {
        // que hit reciba daño de shot, reemplazar prints
        Hitbox h = hit->get_hitbox();
        std::cout << "¡Impacto! Disparo acertó a objeto en (" << h.position.x << ", "
                  << h.position.y << ")\n";
    } else {
        std::cout << "Disparo fallido. No impactó ningún objeto.\n";
    }
}*/

Player::~Player() {}
