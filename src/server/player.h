#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <memory>
#include <string>

class GameWorld;  // forward declaration

#include "common/player_dto.h"
#include "common/settings.h"
#include "server/collidable.h"
#include "server/loadout.h"

class Player: public Collidable {
private:
    const std::string name;
    bool moving_up;
    bool moving_down;
    bool moving_left;
    bool moving_right;
    bool making_action;
    double orientation;
    uint16_t life;
    Loadout loadout;

public:
    Player(const std::string& name, Vector2D<int>& position);

    float get_orientation() const;
    bool is_alive() const;

    void update(GameWorld& game, const float& delta_t);
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
    void buy_ammo(const bool& for_primary);
    const PlayerDTO get_dto() const;

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    ~Player() override;
};

#endif
