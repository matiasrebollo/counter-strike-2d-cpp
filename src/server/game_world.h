#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <list>
#include <map>
#include <memory>
#include <string>

#include "common/game_map.h"
#include "common/game_snapshot.h"
#include "server/collidable.h"
#include "server/game_world_snapshot.h"
#include "server/player.h"
// #include "server/shot.h"

#define PLAYER_SPEED 4
#define TERRORISTS 1
#define COUNTER_TERRORISTS 1

class GameWorld {
private:
    std::map<std::string, std::shared_ptr<Player>> terrorists;
    std::map<std::string, std::shared_ptr<Player>> counter_terrorists;
    std::list<std::shared_ptr<Collidable>> collidables;
    const Rect spawn_zone;

    Vector2D random_position() const;
    Vector2D spawn_position() const;

    bool team_is_dead(const std::map<std::string, std::shared_ptr<Player>>& team) const;

    template <typename PlayerAction>
    void with_player(const std::string& username, PlayerAction action) {
        auto ct_it = counter_terrorists.find(username);
        auto tt_it = terrorists.find(username);
        if (ct_it != counter_terrorists.end()) {
            action(*ct_it->second);
        } else if (tt_it != terrorists.end()) {
            action(*tt_it->second);
        } else {
            throw std::invalid_argument("Username does not correspond to a player in this game.");
        }
    }

    // devuelve la distancia del objeto con el que impactó o 0 si no impactó.
    // double impacts(const Shot& shot, const Collidable& collidable) const;
    // double intersects_segment(const Shot& shot, const Vector2D& seg_start, const Vector2D&
    // seg_end) const;

public:
    const std::string id;

    GameWorld();
    void add_player(const std::string& username);
    const GameMap get_map() const;
    const GameWorldSnapshot get_snapshot() const;
    void update();
    bool tt_are_all_dead() const;
    bool ct_are_all_dead() const;
    void rotate_player(const std::string& username, const double& angle);
    void move_player_up(const std::string& username);
    void move_player_down(const std::string& username);
    void move_player_left(const std::string& username);
    void move_player_right(const std::string& username);
    void make_step_player(Player& player, const Vector2D& step_dir);
    const Collidable* colliding_object_with(const Collidable& coll) const;
    // void shoot(const std::string& username);
    // const Collidable* first_impact(const Shot& shot) const;

    GameWorld(const GameWorld&) = delete;
    GameWorld& operator=(const GameWorld&) = delete;

    ~GameWorld();
};

#endif
