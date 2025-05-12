#include "server/player.h"

#include <algorithm>
#include <iostream>

#include "hitbox.h"

Player::Player(Vector2D pos): hitbox(pos, 0.5f, 0.5f) {}

void Player::step(Vector2D dir, const std::vector<Collidable>& obstacles) {
    Hitbox new_hitbox = hitbox;
    new_hitbox.position = new_hitbox.position + dir * 0.25f;
    std::cout << "Jugador moviendose a: (" << new_hitbox.position.x << "," << new_hitbox.position.y
              << ")\n";

    if (std::any_of(obstacles.begin(), obstacles.end(), [&new_hitbox](const Collidable& obj) {
            return new_hitbox.intersects(obj.get_hitbox());
        })) {
        std::cout << "¡Colisión! No se puede mover ahí.\n";
        return;
    }

    hitbox.position = new_hitbox.position;
}

void Player::print_position() const {
    std::cout << "Jugador en (" << hitbox.position.x << ", " << hitbox.position.y << ")\n";
}

Player::~Player() {}
