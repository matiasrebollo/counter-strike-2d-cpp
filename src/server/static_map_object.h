#ifndef STATIC_MAP_OBJECT_H
#define STATIC_MAP_OBJECT_H

#include "server/collidable.h"

class StaticMapObject: public Collidable {
public:
    StaticMapObject(Vector2D<int> pos, int w, int h): Collidable(pos, w, h) {}
    void receive_damage(const int& /*damage*/) override {}

    StaticMapObject(const StaticMapObject&) = delete;
    StaticMapObject& operator=(const StaticMapObject&) = delete;
};

#endif
