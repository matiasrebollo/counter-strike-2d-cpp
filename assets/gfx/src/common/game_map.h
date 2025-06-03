#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <vector>

#include "common/map_object.h"

struct GameMap {
    std::vector<MapObject> map_objects;
    // const MapType type; ??? tener un "ambiente" para cada mapa?
};


#endif
