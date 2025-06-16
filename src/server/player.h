#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <memory>
#include <optional>
#include <string>

class GameWorld;  // forward declaration

#include "common/player_dto.h"
#include "common/settings.h"
#include "server/collidable.h"
#include "server/loadout.h"
#include "server/shot.h"

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
    std::optional<ShotDTO> shot;
    bool planting_bomb;
    int bonifications;
    int kills;
    int deaths;
    Loadout loadout;

public:
    Player(const std::string& name, Vector2D<int>& position);

    float get_orientation() const;
    bool is_alive() const;
    bool has_bomb() const;
    WeaponType equipped() const;
    void update(GameWorld& game, const float& delta_t);
    void rotate(const double& new_orientation);
    void move_up();
    void move_down();
    void move_left();
    void move_right();
    bool collides_with(const Collidable& other_collidable) const override;
    void receive_bomb(std::shared_ptr<Bomb> bomb);
    void leave_bomb();
    void restart();
    void stop_moving_up();
    void stop_moving_down();
    void stop_moving_left();
    void stop_moving_right();
    void unequip_weapon();
    void equip_primary();
    void equip_secondary();
    void equip_knife();
    void equip_bomb();
    void stop_making_action();
    void make_action();
    void shoot(const Shot& a_shot);
    void receive_damage(const int& damage);
    void count_kill(const int& money_bonification);
    Loadout& get_loadout();
    const PlayerDTO get_dto() const;

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    ~Player() override;
};

#endif
