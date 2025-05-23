#include "server/shot.h"

#include "server/cs2d_game.h"

Shot::Shot(const Vector2D& origin, const float& orientation):
        origin(origin), orientation(orientation) {}

const Collidable* Shot::shoot(const CS2DGame& game) const { return game.first_impact(*this); }

Shot::~Shot() {}
