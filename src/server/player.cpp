#include "server/player.h"

#include <iostream>

#include "server/shot.h"

Player::Player(Vector2D position, Vector2D direction):
        Collidable(position, PLAYER_WIDTH, PLAYER_HEIGHT), direction(direction) {}

void Player::step(Vector2D dir, const std::vector<Collidable>& obstacles) {
    Hitbox old_hitbox = hitbox;

    hitbox.position = hitbox.position + dir * 25;
    std::cout << "Jugador moviéndose a: (" << hitbox.position.x << "," << hitbox.position.y
              << ")\n";

    bool collision = std::any_of(
            obstacles.begin(), obstacles.end(),
            [this](const Collidable& collidable) { return this->collides_with(collidable); });

    if (collision) {
        std::cout << "¡Colisión! No se puede mover ahí.\n";
        hitbox = old_hitbox;
        return;
    }
}

void Player::rotate(Vector2D dir) {
    if (dir.x == 0 && dir.y == 0)
        return;
    direction = dir;
}

void Player::shoot(const std::vector<Collidable>& map_objects) const {
    const Vector2D origin(hitbox.position.x + hitbox.width / 2,
                          hitbox.position.y + hitbox.height / 2);
    Shot shot(origin, direction);

    const Collidable* hit = shot.shoot(map_objects);

    if (hit != nullptr) {
        Hitbox h = hit->get_hitbox();
        std::cout << "¡Impacto! Disparo acertó a objeto en (" << h.position.x << ", "
                  << h.position.y << ")\n";
    } else {
        std::cout << "Disparo fallido. No impactó ningún objeto.\n";
    }
}

void Player::print_position() const {
    std::cout << "Jugador en (" << hitbox.position.x << ", " << hitbox.position.y << ")\n";
}

Player::~Player() {}
