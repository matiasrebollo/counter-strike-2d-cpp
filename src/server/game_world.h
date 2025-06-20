#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "common/game_map.h"
#include "common/game_snapshot.h"
#include "common/settings.h"
#include "server/collidable.h"
#include "server/game_world_snapshot.h"
#include "server/item.h"
#include "server/player.h"
#include "server/shop.h"
#include "server/shot.h"

class GameWorld {
private:
    std::map<std::string, std::shared_ptr<Player>> players;
    std::list<std::shared_ptr<Collidable>> collidables;
    std::vector<Rect> sites;
    std::vector<std::unique_ptr<Item>> items;
    uint64_t next_drop_id;
    std::shared_ptr<Bomb> bomb;
    Shop shop;
    const GameMap game_map;

    void add_collidables();
    void set_sites();

    Vector2D<int> random_spawn_position(const std::vector<Vector2D<int>>& spawn_points) const;
    Vector2D<int> random_ct_spawn_position() const;
    Vector2D<int> random_tt_spawn_position() const;

    bool team_is_dead(std::function<bool(const Player&)> is_in_team) const;

    template <typename PlayerMethod>
    void with_player(const std::string& username, PlayerMethod action) {
        auto it = players.find(username);
        if (it != players.end()) {
            if (it->second->is_alive()) {
                action(*it->second);
            }
        } else {
            throw std::invalid_argument("Username does not correspond to a player in this game.");
        }
    }

    // devuelve el punto de impacto y la distancia del objeto con el que impactó o std::nullopt si
    // no impactó.
    std::optional<std::pair<double, Vector2D<float>>> impacts(const Shot& shot,
                                                              const Collidable& collidable) const;
    std::optional<std::pair<double, Vector2D<float>>> intersects_segment(
            const Shot& shot, const Vector2D<float>& seg_start,
            const Vector2D<float>& seg_end) const;

    bool can_defuse_bomb(const Player& player) const;

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
    void make_bomb_explode();
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
    void make_player_defuse_bomb(const std::string& username);
    void stop_making_player_defuse_bomb(const std::string& username);
    void equip_primary_for(const std::string& username);
    void equip_secondary_for(const std::string& username);
    void equip_knife_for(const std::string& username);
    void equip_bomb_for(const std::string& username);
    void pick_up_item_for(const std::string& username);
    void pick_up_gun_for(Player& player, DroppedGun& dropped_gun);
    void try_pick_up_bomb_for(Player& player, DroppedBomb& dropped_bomb);
    void drop_weapons(Player& player);
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
