#ifndef STATIC_MAP_OBJECT_H
#define STATIC_MAP_OBJECT_H

#include "server/collidable.h"

class StaticMapObject: public Collidable {
public:
    StaticMapObject(Vector2D<int> pos, int w, int h);

    bool collides_with(const Collidable& other_collidable) const override;

    StaticMapObject(const StaticMapObject&) = delete;
    StaticMapObject& operator=(const StaticMapObject&) = delete;

    virtual ~StaticMapObject() override;
};

#endif
