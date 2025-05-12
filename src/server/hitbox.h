#ifndef HITBOX_H
#define HITBOX_H

#include <iostream>

#include "server/vector_2d.h"

struct Hitbox {
    Vector2D position;
    float width, height;

    bool intersects(const Hitbox& other) const {
        return !(position.x + width <= other.position.x ||
                 position.x >= other.position.x + other.width ||
                 position.y + height <= other.position.y ||
                 position.y >= other.position.y + other.height);
    }
};

#endif
