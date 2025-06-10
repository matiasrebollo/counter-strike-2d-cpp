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
#include "server/shot.h"

#define TERRORISTS 1
#define COUNTER_TERRORISTS 1

class GameWorld {
private:
    std::map<std::string, std::shared_ptr<Player>> terrorists;
    std::map<std::string, std::shared_ptr<Player>> counter_terrorists;
    std::list<std::shared_ptr<Collidable>> collidables;
    const Rect spawn_zone;
    const GameMap game_map;

    Vector2D<int> random_spawn_position() const;

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
    double impacts(const Shot& shot, const Collidable& collidable) const;
    double intersects_segment(const Shot& shot, const Vector2D<float>& seg_start,
                              const Vector2D<float>& seg_end) const;

public:
    const std::string id;

    GameWorld();
    void add_player(const std::string& username);
    void stop_players();
    void spawn_players();
    const GameMap get_map() const;
    const GameWorldSnapshot get_snapshot() const;
    void update(const float& delta_t);
    bool tt_are_all_dead() const;
    bool ct_are_all_dead() const;
    void rotate_player(const std::string& username, const double& angle);
    void move_player_up(const std::string& username);
    void move_player_down(const std::string& username);
    void move_player_left(const std::string& username);
    void move_player_right(const std::string& username);
    void make_player_action(const std::string& username);
    void equip_primary_for(const std::string& username);
    void equip_secondary_for(const std::string& username);
    void equip_knife_for(const std::string& username);
    void buy_gun_for(const std::string& username, const GunType& gun);
    void buy_ammo_for(const std::string& username, const bool& for_primary);

    void make_step_player(Player& player, const Vector2D<int>& step);
    const Collidable* colliding_object_with(const Collidable& coll) const;
    Collidable* first_impact(const Shot& shot, const Player& shooter) const;

    GameWorld(const GameWorld&) = delete;
    GameWorld& operator=(const GameWorld&) = delete;

    ~GameWorld();
};

#endif
