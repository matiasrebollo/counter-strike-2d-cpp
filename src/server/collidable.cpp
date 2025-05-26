#include "server/collidable.h"

#include <iostream>

Collidable::Collidable(Vector2D pos, int w, int h): rect(pos, w, h) {}

bool Collidable::collides_with(const Collidable& other_collidable) const {
    return this->rect.intersects_with(other_collidable.rect);
}

Rect Collidable::get_rect() const { return rect; }

Collidable::~Collidable() {}
