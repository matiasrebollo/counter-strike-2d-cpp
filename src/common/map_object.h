#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include "common/vector_2d.h"

enum MapObjectType {
    BOX,
    NONE_BLOCK,
    AZTEC_STONE_WALL_1,
    AZTEC_STONE_WALL_2,
    AZTEC_STONE_WALL_3,
    AZTEC_STONE_WALL_4,
    AZTEC_STONE_WALL_5,
    AZTEC_STONE_WALL_6,
    AZTEC_STONE_WALL_7,
    AZTEC_STONE_WALL_8
};

struct MapObject {
    const Vector2D position;
    const int width;
    const int height;
    const MapObjectType type;
};


#endif
