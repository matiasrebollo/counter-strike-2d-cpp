#include "server/shot.h"

#include "server/cs2d_game.h"

Shot::Shot(const Vector2D& origin, const Vector2D& direction):
        origin(origin), direction(direction) {}

const Collidable* Shot::shoot(const CS2DGame& game) const { return game.first_impact(*this); }

Shot::~Shot() {}
