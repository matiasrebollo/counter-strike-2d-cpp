#ifndef MAP_OBJECT_H
#define MAP_OBJECT_H

#include "common/vector_2d.h"

enum MapObjectType { BOX, WALL, CAR };

struct MapObject {
    const Vector2D position;
    const int width;
    const int height;
    const MapObjectType type;
};


#endif
