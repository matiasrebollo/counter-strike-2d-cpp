#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "server/hitbox.h"

class Collidable {
protected:
    Hitbox hitbox;

public:
    Collidable(Vector2D pos, int w, int h);

    virtual bool collides_with(const Collidable& other_collidable) const;

    virtual Hitbox get_hitbox() const;

    Collidable(const Collidable&) = delete;
    Collidable& operator=(const Collidable&) = delete;

    virtual ~Collidable();
};

#endif
