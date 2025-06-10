#include "server/shot.h"

#include "server/game_world.h"

Shot::Shot(const Vector2D<int>& origin, const float& orientation):
        origin(origin), orientation(orientation) {}

Collidable* Shot::shoot(const GameWorld& game, const Player& shooter) {
    return game.first_impact(*this, shooter);
}

Shot::~Shot() {}
