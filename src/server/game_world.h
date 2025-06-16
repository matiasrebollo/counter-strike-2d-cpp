#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "common/game_map.h"
#include "common/game_snapshot.h"
#include "common/settings.h"
#include "server/collidable.h"
#include "server/game_world_snapshot.h"
#include "server/player.h"
#include "server/shop.h"
#include "server/shot.h"

class GameWorld {
private:
    std::map<std::string, std::shared_ptr<Player>> terrorists;
    std::map<std::string, std::shared_ptr<Player>> counter_terrorists;
    std::list<std::shared_ptr<Collidable>> collidables;
    std::vector<Rect> sites;
    std::shared_ptr<Bomb> bomb;
    std::optional<Vector2D<int>> bomb_position;
    Shop shop;
    const GameMap game_map;

    void add_collidables();
    void set_sites();

    Vector2D<int> random_spawn_position(const std::vector<Vector2D<int>>& spawn_points) const;
    Vector2D<int> random_ct_spawn_position() const;
    Vector2D<int> random_tt_spawn_position() const;

    bool team_is_dead(const std::map<std::string, std::shared_ptr<Player>>& team) const;

    template <typename PlayerMethod>
    void with_player(const std::string& username, PlayerMethod action) {

        auto ct_it = counter_terrorists.find(username);
        auto tt_it = terrorists.find(username);
        if (ct_it != counter_terrorists.end()) {
            if (ct_it->second->is_alive())
                action(*ct_it->second);
        } else if (tt_it != terrorists.end()) {
            if (tt_it->second->is_alive())
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

    explicit GameWorld(const std::string& map_filename);
    void add_player(const std::string& username);
    void swap_teams();
    void restart_players();
    void spawn_players();
    const GameMap get_map() const;
    const ShopInfoDTO get_shop_info() const;
    const GameWorldSnapshot get_snapshot() const;
    void update(const float& delta_t);
    bool bomb_just_planted() const;
    int bomb_detonation_time() const;
    bool bomb_not_planted() const;
    bool bomb_exploded() const;
    bool bomb_defused() const;
    void defuse_bomb();
    void plant_bomb(Player& terrorist);
    bool tt_are_all_dead() const;
    bool ct_are_all_dead() const;
    void rotate_player(const std::string& username, const double& angle);
    bool on_site(const Player& player) const;
    void move_player_up(const std::string& username);
    void move_player_down(const std::string& username);
    void move_player_left(const std::string& username);
    void move_player_right(const std::string& username);
    void stop_moving_player_up(const std::string& username);
    void stop_moving_player_down(const std::string& username);
    void stop_moving_player_left(const std::string& username);
    void stop_moving_player_right(const std::string& username);
    void make_player_action(const std::string& username);
    void stop_making_player_action(const std::string& username);
    void equip_primary_for(const std::string& username);
    void equip_secondary_for(const std::string& username);
    void equip_knife_for(const std::string& username);
    void equip_bomb_for(const std::string& username);
    void buy_gun_for(const std::string& username, const GunType& gun);
    void buy_ammo_for(const std::string& username, const bool& for_primary);

    void make_step_player(Player& player, const Vector2D<int>& step);
    const Collidable* colliding_object_with(const Collidable& coll) const;
    void calculate_shot(Shot& shot, const Player& shooter) const;

    GameWorld(const GameWorld&) = delete;
    GameWorld& operator=(const GameWorld&) = delete;

    ~GameWorld();
};

#endif
