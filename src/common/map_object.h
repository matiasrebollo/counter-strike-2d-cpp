#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include <string>
#include <vector>

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
    std::vector<Vector2D> positions;
    const MapObjectType type;
    const bool collidable;
};

std::string map_obj_to_str(MapObjectType obj);
MapObjectType str_to_map_obj(const std::string& str);

#endif
