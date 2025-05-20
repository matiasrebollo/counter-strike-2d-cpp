#include "server/player.h"

#include <iostream>

#include "server/cs2d_game.h"

Player::Player(Vector2D& position, Vector2D& direction, ClientSender& sender):
        Collidable(position, PLAYER_WIDTH, PLAYER_HEIGHT),
        direction(direction),
        life(PLAYER_INITIAL_LIFE),
        sender(sender) {}

Vector2D Player::get_direction() const { return direction; }

uint16_t Player::get_life() const { return life; }

void Player::step(const Vector2D& step_dir, const CS2DGame& game) {
    Hitbox old_hitbox = Hitbox(hitbox);

    hitbox.position = hitbox.position + step_dir * PLAYER_SPEED;

    const bool collision = game.is_player_in_valid_position(*this);

    if (collision) {
        hitbox = old_hitbox;
        return;
    }
}

void Player::send_map(const GameMap& map) { sender.send_map(map); }
void Player::send_snapshot(const Snapshot& snapshot) { sender.push(snapshot); }

void Player::rotate(const Vector2D& new_dir) { this->direction = new_dir; }

void Player::shoot(const CS2DGame& game) const {
    const Vector2D origin(hitbox.position.x + hitbox.width / 2,
                          hitbox.position.y + hitbox.height / 2);
    Shot shot(origin, direction);

    const Collidable* hit = shot.shoot(game);

    if (hit != nullptr) {
        // que hit reciba daño de shot, reemplazar prints
        Hitbox h = hit->get_hitbox();
        std::cout << "¡Impacto! Disparo acertó a objeto en (" << h.position.x << ", "
                  << h.position.y << ")\n";
    } else {
        std::cout << "Disparo fallido. No impactó ningún objeto.\n";
    }
}

Player::~Player() {}
