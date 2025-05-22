#include "server/collidable.h"

#include <iostream>

Collidable::Collidable(Vector2D pos, int w, int h): hitbox(pos, w, h) {}

bool Collidable::collides_with(const Collidable& other_collidable) const {
    return this->hitbox.intersects_with(other_collidable.hitbox);
}

Hitbox Collidable::get_hitbox() const { return hitbox; }

Collidable::~Collidable() {}
