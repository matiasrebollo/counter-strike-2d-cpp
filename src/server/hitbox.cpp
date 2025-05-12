#include "server/hitbox.h"

bool Hitbox::intersects(const Hitbox& other) const {
    return !(position.x + width <= other.position.x ||
             position.x >= other.position.x + other.width ||
             position.y + height <= other.position.y ||
             position.y >= other.position.y + other.height);
}
