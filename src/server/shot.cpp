#include "server/shot.h"

#include <cmath>

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

std::optional<ShotDTO> Shot::get_dto() const {
    if (!impact_info)
        return std::nullopt;

    Vector2D<float> point = impact_info->second;
    Vector2D<int> point_int(static_cast<int>(std::round(point.x)),
                            static_cast<int>(std::round(point.y)));

    return ShotDTO{point_int};
}

Shot::~Shot() {}
