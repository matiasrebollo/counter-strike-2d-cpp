#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <memory>

#include "server/loadout_manager.h"
class GameWorld;  // forward declaration

#include "server/collidable.h"

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32
#define PLAYER_INITIAL_LIFE 100

class Player: public Collidable {
private:
    bool moving_up;
    bool moving_down;
    bool moving_left;
    bool moving_right;
    double orientation;
    uint16_t life;
    LoadoutManager loadout;

    void step(const Vector2D& step_dir, GameWorld& game);

public:
    explicit Player(Vector2D& position);

    float get_orientation() const;
    uint16_t get_life() const;
    bool is_alive() const;

    void update(GameWorld& game);
    void rotate(const double& new_orientation);
    void move_up();
    void move_down();
    void move_left();
    void move_right();
    // void shoot(const CS2DGame& game) const;

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    ~Player() override;
};

#endif
