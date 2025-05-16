#include "server/hitbox.h"

Hitbox::Hitbox(Vector2D position, int width, int height):
        position(position), width(width), height(height) {}

bool Hitbox::intersects_with(const Hitbox& other_hitbox) const {
    return !(position.x + width <= other_hitbox.position.x ||
             position.x >= other_hitbox.position.x + other_hitbox.width ||
             position.y + height <= other_hitbox.position.y ||
             position.y >= other_hitbox.position.y + other_hitbox.height);
}

Hitbox::~Hitbox() {}
