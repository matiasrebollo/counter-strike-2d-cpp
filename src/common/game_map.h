#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <vector>

#include "common/map_object.h"

struct GameMap {
    int width;
    int height;
    std::vector<MapObject> map_objects;
};


#endif
