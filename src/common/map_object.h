#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include <string>
#include <vector>

#include "common/vector_2d.h"

#define NONE_BLOCK -1

struct MapObject {
    const std::vector<Vector2D> positions;
    const int type;
    const bool collidable;
};

#endif
