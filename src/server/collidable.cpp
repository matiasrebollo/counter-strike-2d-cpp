#include "server/collidable.h"

#include <iostream>

Collidable::Collidable(Vector2D pos, float w, float h): hitbox(pos, w, h) {}

const Hitbox& Collidable::get_hitbox() const { return hitbox; }

Collidable::~Collidable() {}
