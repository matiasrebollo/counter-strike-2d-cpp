#ifndef SHOT_H
#define SHOT_H

#include <list>
#include <memory>
#include <optional>

#include "server/collidable.h"

class CS2DGame;  // forward declaration

class Shot {
public:
    const Vector2D origin;
    const Vector2D direction;

    Shot(const Vector2D& origin, const Vector2D& direction);

    const Collidable* shoot(const CS2DGame& game) const;

    Shot(const Shot&) = delete;
    Shot& operator=(const Shot&) = delete;

    ~Shot();
};

#endif
