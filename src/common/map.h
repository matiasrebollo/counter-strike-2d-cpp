#ifndef MAP_H
#define MAP_H

#include <vector>

#include "common/map_object.h"

struct Map {
    const std::vector<MapObject> map_objects;
    // const MapType type; ??? tener un "ambiente" para cada mapa?
};


#endif
