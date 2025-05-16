#ifndef HITBOX_H
#define HITBOX_H

#include "server/vector_2d.h"

class Hitbox {
public:
    Vector2D position;
    int width, height;

    Hitbox(Vector2D position, int width, int height);

    bool intersects_with(const Hitbox& other_hitbox) const;

    // Permito la copia para una Hitbox
    Hitbox(const Hitbox&) = default;
    Hitbox& operator=(const Hitbox&) = default;

    ~Hitbox();
};

#endif
