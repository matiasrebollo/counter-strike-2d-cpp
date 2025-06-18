#ifndef SHOT_H
#define SHOT_H

#include <list>
#include <memory>
#include <optional>
#include <utility>

#include "server/collidable.h"

class GameWorld;  // forward declaration
class Player;     // forward declaration

class Shot {
public:
    const Vector2D<int> origin;
    const float orientation;
    Collidable* hit;
    std::optional<std::pair<double, Vector2D<float>>> impact_info;

    Shot(const Vector2D<int>& origin, const float& orientation);

    void shoot(const GameWorld& game, Player& shooter);

    Shot(const Shot&) = delete;
    Shot& operator=(const Shot&) = delete;

    ~Shot();
};

#endif
