#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <list>
#include <map>
#include <memory>
#include <string>

#include "common/game_map.h"
#include "common/game_snapshot.h"
#include "server/collidable.h"
#include "server/player.h"
// #include "server/shot.h"

#define PLAYER_SPEED 4

class GameWorld {
private:
    std::map<std::string, std::shared_ptr<Player>> players;
    std::list<std::shared_ptr<Collidable>> collidables;
    const Rect spawn_zone;

    Vector2D random_position() const;
    Vector2D spawn_position() const;

    template <typename PlayerAction>
    void with_player(const std::string& username, PlayerAction action) {
        auto it = players.find(username);
        if (it != players.end()) {
            action(*it->second);
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
    const Snapshot get_snapshot() const;
    void update();
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
