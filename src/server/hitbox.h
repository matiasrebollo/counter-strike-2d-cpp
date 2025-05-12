#ifndef HITBOX_H
#define HITBOX_H

#include "server/vector_2d.h"

struct Hitbox {
    Vector2D position;
    float width, height;

    bool intersects(const Hitbox& other) const;

    ~Hitbox() = default;
};

#endif
