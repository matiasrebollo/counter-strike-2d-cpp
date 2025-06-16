#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <map>
#include <vector>

#include "common/map_object.h"
#include "common/vector_2d.h"
#include "common/weapon_type.h"

struct GameMap {
    int width;
    int height;
    Background background;
    std::vector<MapObject> map_objects;
    std::vector<Vector2D<int>> ct_spawns;
    std::vector<Vector2D<int>> tt_spawns;
    std::vector<Vector2D<int>> sites;
    std::map<GunType, std::vector<Vector2D<int>>> guns;
};

#endif
