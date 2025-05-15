#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

// #include "server/loadout_manager.h"
#include "server/collidable.h"
#include "server/hitbox.h"

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 50

class Player: public Collidable {
private:
    Vector2D direction;
    // LoadoutManager loadout;

public:
    Player(Vector2D position, Vector2D direction);

    void step(Vector2D direction, const std::vector<Collidable>& obstacles);
    void rotate(Vector2D direction);
    void shoot(const std::vector<Collidable>& obstacles) const;

    void print_position() const;

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    Player(Player&&) noexcept = default;
    Player& operator=(Player&&) noexcept = default;

    ~Player();
};

#endif
