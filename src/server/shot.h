#ifndef SHOT_H
#define SHOT_H

#include <list>
#include <memory>
#include <optional>

#include "server/collidable.h"

class Shot {
private:
    Vector2D origin;
    Vector2D direction;

    // devuelve la distancia del objeto con el que impactó o 0 si no impactó.
    double impacts(const Collidable& collidable) const;
    double intersects_segment(const Vector2D& seg_start, const Vector2D& seg_end) const;


public:
    Shot(Vector2D origin, Vector2D direction);

    const Collidable* shoot(const std::list<std::shared_ptr<Collidable>>& obstacless) const;

    Shot(const Shot&) = delete;
    Shot& operator=(const Shot&) = delete;

    ~Shot();
};

#endif
