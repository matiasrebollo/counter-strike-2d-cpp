#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <memory>

// #include "server/loadout_manager.h"
class CS2DGame;  // forward declaration

#include "server/collidable.h"

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 50

class Player: public Collidable {
private:
    Vector2D direction;
    // ClientReceiver con el que comunicarse!!
    // LoadoutManager loadout;

public:
    Player(Vector2D& position, Vector2D& direction);

    void step(const Vector2D& step_dir, const CS2DGame& game);
    void rotate(const Vector2D& new_dir);
    void shoot(const CS2DGame& game) const;

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    ~Player() override;
};

#endif
