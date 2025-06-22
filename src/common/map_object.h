#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include <string>
#include <vector>

#include "common/vector_2d.h"

#define NONE_BLOCK -1

struct MapObject {
    std::vector<Vector2D<int>> positions;
    int type;
    bool collidable;
};

enum Background {
    AZTEC_BACKGROUND = 0,
    DUST_BACKGROUND = 1,
    GRASS_BACKGROUND = 2,
    BLOOD_BACKGROUND = 3,
    INFERNO_BACKGROUND = 4,
    LAVA_BACKGROUND = 5,
    MARS_BACKGROUND = 6,
    NUKE_BACKGROUND = 7,
    OFFICE_BACKGROUND = 8,
    SAND1_BACKGROUND = 9,
    SAND1_NIGHT_BACKGROUND = 10,
    SNOW_BACKGROUND = 11,
    SPACE_BACKGROUND = 12,
    STONE1_BACKGROUND = 13,
    TOXIC_BACKGROUND = 14,
    WATER1_BACKGROUND = 15,
    WATER2_BACKGROUND = 16,
    WATER3_BACKGROUND = 17,
    WATER4_BACKGROUND = 18
};

#endif
