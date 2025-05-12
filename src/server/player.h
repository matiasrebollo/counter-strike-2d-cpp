#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "server/collidable.h"
#include "server/hitbox.h"

class Player {
private:
    Hitbox hitbox;

public:
    explicit Player(Vector2D pos);

    void step(Vector2D dir, const std::vector<Collidable>& obstacles);

    void print_position() const;

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    Player(Player&&) noexcept = default;
    Player& operator=(Player&&) noexcept = default;

    ~Player();
};

#endif
