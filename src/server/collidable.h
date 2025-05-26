#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "server/rect.h"

class Collidable {
protected:
    Rect rect;

public:
    Collidable(Vector2D pos, int w, int h);

    virtual bool collides_with(const Collidable& other_collidable) const;

    virtual Rect get_rect() const;

    Collidable(const Collidable&) = delete;
    Collidable& operator=(const Collidable&) = delete;

    virtual ~Collidable();
};

#endif
