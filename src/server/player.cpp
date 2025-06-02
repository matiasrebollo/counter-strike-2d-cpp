#include "server/player.h"

#include <iostream>

#include "server/game_world.h"

Player::Player(Vector2D& position):
        Collidable(position, PLAYER_WIDTH, PLAYER_HEIGHT),
        moving_up(false),
        moving_down(false),
        moving_left(false),
        moving_right(false),
        life(PLAYER_INITIAL_LIFE) {}

float Player::get_orientation() const { return orientation; }
uint16_t Player::get_life() const { return life; }

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
    // si esta disparando, ...
}

/*void Player::step(const Vector2D& step_dir, GameWorld& game) {
    Rect old_rect = Rect(this->rect);
    std::cout << "posicion anterior: x: " << old_rect.position.x << ", y: " << old_rect.position.y
              << std::endl;

    this->rect.position = rect.position + step_dir * PLAYER_SPEED;
    std::cout << "me quiero mover a: x: " << this->rect.position.x
              << ", y: " << this->rect.position.y << std::endl;
    const bool collision = game.is_player_not_in_valid_position(*this);
    if (collision) {
        std::cout << "colision!" << std::endl;
        // ver de "avanzar lo mas posible" en vez de calcelar el movimiento???
        this->rect = old_rect;
        return;
    }
    std::cout << "posicion final: x: " << this->rect.position.x << ", y: " << this->rect.position.y
              << std::endl;
    std::cout << "\n";
}*/

void Player::step(const Vector2D& step_dir, GameWorld& game) {
    std::cout << "posicion anterior: x: " << this->rect.position.x
              << ", y: " << this->rect.position.y << std::endl;

    game.make_step_player(*this, step_dir);
    std::cout << "posicion final: x: " << this->rect.position.x << ", y: " << this->rect.position.y
              << std::endl;
    std::cout << "\n";
}

void Player::move_up() { moving_up = !moving_up; }
void Player::move_down() { moving_down = !moving_down; }
void Player::move_left() { moving_left = !moving_left; }
void Player::move_right() { moving_right = !moving_right; }
void Player::rotate(const double& new_orientation) { this->orientation = new_orientation; }

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
