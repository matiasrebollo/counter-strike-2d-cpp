#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "server/hitbox.h"

class Collidable {
protected:
    Hitbox hitbox;

public:
    Collidable(Vector2D pos, float w, float h);
    const Hitbox& get_hitbox() const;

    Collidable(const Collidable&) = delete;
    Collidable& operator=(const Collidable&) = delete;

    Collidable(Collidable&&) noexcept = default;
    Collidable& operator=(Collidable&&) noexcept = default;

    ~Collidable();
};

#endif
