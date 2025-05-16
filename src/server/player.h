#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <memory>

// #include "server/loadout_manager.h"
#include "server/collidable.h"
#include "server/hitbox.h"

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 50

class Player: public Collidable {
private:
    Vector2D direction;
    // Receiver o Sender con el que comunicarse
    // LoadoutManager loadout;

public:
    Player(Vector2D position, Vector2D direction);

    void step(Vector2D direction, const std::list<std::shared_ptr<Collidable>>& obstacles);
    void rotate(Vector2D direction);
    void shoot(const std::list<std::shared_ptr<Collidable>>& obstacles) const;

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    Player(Player&&) noexcept = default;
    Player& operator=(Player&&) noexcept = default;

    ~Player() override;
};

#endif
