#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "server/rect.h"

class Collidable {
public:
    Rect rect;

    Collidable(Vector2D<int> pos, int w, int h): rect(pos, w, h) {}

    virtual bool collides_with(const Collidable& other_collidable) const = 0;

    Collidable(const Collidable&) = delete;
    Collidable& operator=(const Collidable&) = delete;

    virtual ~Collidable() = default;
};

#endif
