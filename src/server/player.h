#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <memory>

// #include "server/loadout_manager.h"
class CS2DGame;  // forward declaration

#include "server/client_sender.h"
#include "server/collidable.h"

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32
#define PLAYER_SPEED 2
#define PLAYER_INITIAL_LIFE 1000

class Player: public Collidable {
private:
    bool moving_up;
    bool moving_down;
    bool moving_left;
    bool moving_right;
    float orientation;
    uint16_t life;
    ClientSender& sender;

    // LoadoutManager loadout;

    void step(const Vector2D& step_dir, CS2DGame& game);

public:
    Player(Vector2D& position, float& orientation, ClientSender& sender);

    float get_orientation() const;
    uint16_t get_life() const;

    void send_map(const GameMap& map);
    void send_snapshot(const Snapshot& map);

    void update(CS2DGame& game);
    void rotate(const float& new_orientation);
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
