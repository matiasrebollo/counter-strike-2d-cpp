#ifndef SHOT_H
#define SHOT_H

#include <list>
#include <memory>
#include <optional>

#include "server/collidable.h"

class GameWorld;  // forward declaration
class Player;     // forward declaration

class Shot {
public:
    const Vector2D<int> origin;
    const float orientation;

    Shot(const Vector2D<int>& origin, const float& orientation);

    Collidable* shoot(const GameWorld& game, const Player& shooter);

    Shot(const Shot&) = delete;
    Shot& operator=(const Shot&) = delete;

    ~Shot();
};

#endif
