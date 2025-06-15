#include "server/shot.h"

#include "server/game_world.h"

Shot::Shot(const Vector2D<int>& origin, const float& orientation):
        origin(origin), orientation(orientation), hit(nullptr), distance(-1) {}

void Shot::shoot(const GameWorld& game, const Player& shooter) {
    game.calculate_shot(*this, shooter);
    if (hit == nullptr || distance < 0) {
        throw std::runtime_error("El tiro no puede no impractar a ningún colisionable del juego");
    }
    shooter.shoot(*this);
}

Shot::~Shot() {}
