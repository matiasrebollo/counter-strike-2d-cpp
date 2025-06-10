#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include <string>
#include <vector>

#include "common/vector_2d.h"

#define NONE_BLOCK -1

struct MapObject {
    std::vector<Vector2D> positions;
    int type;
    bool collidable;
};

enum Background { AZTEC_BACKGROUND = 0, DUST_BACKGROUND = 1, GRASS_BACKGROUND = 2 };

#endif
