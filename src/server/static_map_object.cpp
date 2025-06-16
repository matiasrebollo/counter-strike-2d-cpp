#include "server/static_map_object.h"


StaticMapObject::StaticMapObject(Vector2D<int> pos, int w, int h): Collidable(pos, w, h) {}

bool StaticMapObject::collides_with(const Collidable& other_collidable) const {
    return this->rect.intersects_with(other_collidable.rect);
}

StaticMapObject::~StaticMapObject() {}
