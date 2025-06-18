#include "server/shot.h"

#include "server/game_world.h"

Shot::Shot(const Vector2D<int>& origin, const float& orientation):
        origin(origin), orientation(orientation), hit(nullptr), impact_info(std::nullopt) {}

void Shot::shoot(const GameWorld& game, Player& shooter) {
    game.calculate_shot(*this, shooter);
    if (hit == nullptr || impact_info->first < 0) {
        throw std::runtime_error("El tiro no puede no impactar a ningún colisionable del juego");
    }
    shooter.shoot(*this);
}

Shot::~Shot() {}
