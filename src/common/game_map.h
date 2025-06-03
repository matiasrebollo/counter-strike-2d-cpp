#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <vector>

#include "common/map_object.h"
#include "common/vector_2d.h"

struct GameMap {
    int width;
    int height;
    std::vector<MapObject> map_objects;
    std::vector<Vector2D> ct_spawns;
    std::vector<Vector2D> tt_spawns;
    std::vector<Vector2D> sites;
};


#endif
