#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <memory>
#include <string>


class GameWorld;  // forward declaration

#include "common/player_dto.h"
#include "server/collidable.h"
#include "server/loadout.h"

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32
#define PLAYER_INITIAL_LIFE 100

class Player: public Collidable {
private:
    const std::string name;
    bool moving_up;
    bool moving_down;
    bool moving_left;
    bool moving_right;
    double orientation;
    uint16_t life;
    Loadout loadout;

    void step(const Vector2D& step_dir, GameWorld& game);

public:
    Player(const std::string& name, Vector2D& position);

    float get_orientation() const;
    uint16_t get_life() const;
    bool is_alive() const;

    void update(GameWorld& game);
    void rotate(const double& new_orientation);
    void move_up();
    void move_down();
    void move_left();
    void move_right();
    void stop();
    void equip_primary();
    void equip_secondary();
    void equip_knife();
    void make_action();
    void buy_gun(const GunType& gun);
    void buy_ammo(const uint16_t& ammo, const bool& for_primary);
    const PlayerDTO get_dto() const;

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    ~Player() override;
};

#endif
