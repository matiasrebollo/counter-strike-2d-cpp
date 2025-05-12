#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "server/collidable.h"
#include "server/hitbox.h"

class Player {
private:
    Hitbox hitbox;
    Vector2D direction;

    bool shoot_intersects_hitbox(const Vector2D& shoot_origin, const Vector2D& shoot_dir,
                                 const Hitbox& hitbox) const;
    bool shoot_intersects_segment(const Vector2D& shoot_origin, const Vector2D& shoot_dir,
                                  const Vector2D& seg_start, const Vector2D& seg_end) const;

public:
    explicit Player(Vector2D pos);

    void step(Vector2D dir, const std::vector<Collidable>& obstacles);
    void rotate(Vector2D dir);
    void shoot(const std::vector<Collidable>& obstacles) const;

    void print_position() const;

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    Player(Player&&) noexcept = default;
    Player& operator=(Player&&) noexcept = default;

    ~Player();
};

#endif
