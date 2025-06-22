#include "server/game_world.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

#include "common/yaml_parser.h"
#include "server/static_map_object.h"

void GameWorld::add_collidables() {
    for (const auto& block: game_map.map_objects) {
        if (block.collidable) {
            for (const auto& vec: block.positions) {
                collidables.emplace_back(std::make_shared<StaticMapObject>(
                        Vector2D<int>(vec.x * BLOCK_THICKNESS, vec.y * BLOCK_THICKNESS),
                        BLOCK_THICKNESS, BLOCK_THICKNESS));
            }
        }
    }

    // paredes invisibles al borde del mapa
    collidables.emplace_back(std::make_shared<StaticMapObject>(
            Vector2D<int>(0, -BLOCK_THICKNESS), game_map.width * BLOCK_THICKNESS, BLOCK_THICKNESS));
    collidables.emplace_back(
            std::make_shared<StaticMapObject>(Vector2D<int>(game_map.width * BLOCK_THICKNESS, 0),
                                              BLOCK_THICKNESS, game_map.height * BLOCK_THICKNESS));
    collidables.emplace_back(
            std::make_shared<StaticMapObject>(Vector2D<int>(0, game_map.height * BLOCK_THICKNESS),
                                              game_map.width * BLOCK_THICKNESS, BLOCK_THICKNESS));
    collidables.emplace_back(std::make_shared<StaticMapObject>(Vector2D<int>(-BLOCK_THICKNESS, 0),
                                                               BLOCK_THICKNESS,
                                                               game_map.height * BLOCK_THICKNESS));
}

void GameWorld::set_sites() {
    for (const auto& site: game_map.sites) {
        Vector2D<int> new_site_pos(site.x * BLOCK_THICKNESS, site.y * BLOCK_THICKNESS);
        Rect new_site(new_site_pos, BLOCK_THICKNESS, BLOCK_THICKNESS);
        this->sites.push_back(new_site);
    }
}

void GameWorld::set_items() {
    for (const auto& [gun_type, positions]: game_map.guns) {
        for (const auto& pos: positions) {
            Vector2D<int> item_pos(pos.x * BLOCK_THICKNESS, pos.y * BLOCK_THICKNESS);
            Rect item_rect(item_pos, ITEM_THICKNESS, ITEM_THICKNESS);
            items.emplace_back(std::make_unique<DroppedGun>(
                    item_rect, std::move(Gun::new_gun(gun_type)), next_drop_id++));
        }
    }
}

GameWorld::GameWorld(const std::string& map_filename):
        next_drop_id(0),
        bomb(std::make_shared<Bomb>()),
        shop(),
        game_map(YamlParser().yaml_to_game_map(PATH_FOLDER_MAPS + map_filename + ".yaml")),
        COUNTER_TERRORISTS(Settings::getInstance().get_counter_terrorists_number()),
        TERRORISTS(Settings::getInstance().get_terrorists_number()) {
    size_t min_spawns_size = std::min(game_map.ct_spawns.size(), game_map.tt_spawns.size());
    size_t max_team_size = std::max(COUNTER_TERRORISTS, TERRORISTS);
    if (max_team_size > min_spawns_size)
        throw std::runtime_error("Not enough spawns on map");
    add_collidables();
    set_sites();
    set_items();
}

// spawn_points deben ser suficientes como para que eventualmente se pueda spawnear a un jugador y
// no quedarse buscando.
Vector2D<int> GameWorld::random_spawn_position(
        const std::vector<Vector2D<int>>& spawn_points) const {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(0, spawn_points.size() - 1);
    Vector2D<int> grid_pos = spawn_points[dis(gen)];

    int x = grid_pos.x * BLOCK_THICKNESS;
    int y = grid_pos.y * BLOCK_THICKNESS;

    if (BLOCK_THICKNESS > PLAYER_THICKNESS) {
        x += (BLOCK_THICKNESS - PLAYER_THICKNESS) / 2;
        y += (BLOCK_THICKNESS - PLAYER_THICKNESS) / 2;
    }

    return Vector2D<int>(x, y);
}

Vector2D<int> GameWorld::random_ct_spawn_position() const {
    return random_spawn_position(game_map.ct_spawns);
}
Vector2D<int> GameWorld::random_tt_spawn_position() const {
    return random_spawn_position(game_map.tt_spawns);
}

void GameWorld::add_player(const std::string& username) {
    Vector2D<int> default_position(-100, -100);
    auto player = std::make_shared<Player>(username, default_position);

    size_t cts = 0;
    size_t tts = 0;
    for (const auto& [name, p]: players) {
        if (p->is_ct()) {
            ++cts;
        } else if (p->is_tt()) {
            ++tts;
        }
    }

    if (cts < COUNTER_TERRORISTS && tts < TERRORISTS) {
        if (cts <= tts) {
            player->change_team(CT);
        } else {
            player->change_team(TT);
        }
    } else if (cts < COUNTER_TERRORISTS) {
        player->change_team(CT);
    } else if (tts < TERRORISTS) {
        player->change_team(TT);
    } else {
        throw std::runtime_error("No hay lugar para más jugadores");
    }

    players[username] = player;
    collidables.push_back(player);
}

void GameWorld::swap_teams() {
    for (auto& [username, player]: players) {
        if (player->is_ct()) {
            player->change_team(TT);
        } else if (player->is_tt()) {
            player->change_team(CT);
        }
        Loadout& loadout = player->get_loadout();
        loadout.reset(false);
    }
}

void GameWorld::restart_players() {
    std::vector<std::shared_ptr<Player>> terrorist_players;

    for (auto& [_, player]: players) {
        player->restart();
        if (player->is_tt()) {
            terrorist_players.push_back(player);
        }
    }

    bomb->restart();

    if (!terrorist_players.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, terrorist_players.size() - 1);
        size_t index = dist(gen);
        terrorist_players[index]->receive_bomb(bomb);
    }
}

void GameWorld::restart_items() {
    items.clear();
    next_drop_id = 0;
    set_items();
}

void GameWorld::spawn_players() {
    for (auto& [_, player]: players) {
        Vector2D<int> position;

        if (player->is_tt()) {
            position = random_tt_spawn_position();
        } else if (player->is_ct()) {
            position = random_ct_spawn_position();
        } else {
            continue;
        }

        player->rect.position = position;

        while (colliding_object_with(*player)) {
            position = player->is_tt() ? random_tt_spawn_position() : random_ct_spawn_position();
            player->rect.position = position;
        }
    }
}

const GameMap GameWorld::get_map() const { return this->game_map; }

const ShopInfoDTO GameWorld::get_shop_info() const { return this->shop.get_shop_info(); }

const GameWorldSnapshot GameWorld::get_snapshot() const {
    std::vector<PlayerDTO> ct;
    std::vector<PlayerDTO> tt;

    for (const auto& [_, player]: players) {
        if (player->is_ct()) {
            ct.push_back(player->get_dto());
        } else if (player->is_tt()) {
            tt.push_back(player->get_dto());
        }
    }

    std::vector<ItemDTO> item_dtos;
    for (const auto& item: items) {
        item_dtos.push_back(item->get_dto());
    }

    return GameWorldSnapshot{bomb->get_status(), bomb->get_plantation_position(), ct, tt,
                             item_dtos};
}

void GameWorld::rotate_player(const std::string& username, const double& angle) {
    with_player(username, [angle](Player& p) { p.rotate(angle); });
}

void GameWorld::move_player_up(const std::string& username) {
    with_player(username, [](Player& p) { p.move_up(); });
}

void GameWorld::move_player_down(const std::string& username) {
    with_player(username, [](Player& p) { p.move_down(); });
}

void GameWorld::move_player_left(const std::string& username) {
    with_player(username, [](Player& p) { p.move_left(); });
}

void GameWorld::move_player_right(const std::string& username) {
    with_player(username, [](Player& p) { p.move_right(); });
}

void GameWorld::stop_moving_player_up(const std::string& username) {
    with_player(username, [](Player& p) { p.stop_moving_up(); });
}

void GameWorld::stop_moving_player_down(const std::string& username) {
    with_player(username, [](Player& p) { p.stop_moving_down(); });
}

void GameWorld::stop_moving_player_left(const std::string& username) {
    with_player(username, [](Player& p) { p.stop_moving_left(); });
}

void GameWorld::stop_moving_player_right(const std::string& username) {
    with_player(username, [](Player& p) { p.stop_moving_right(); });
}

void GameWorld::make_player_action(const std::string& username) {
    with_player(username, [](Player& p) {
        if (p.equipped() == BOMB && !p.is_on_site())
            return;
        p.make_action();
    });
}

void GameWorld::stop_making_player_action(const std::string& username) {
    with_player(username, [](Player& p) {
        if (p.equipped() == BOMB && !p.is_on_site())
            return;
        p.stop_making_action();
    });
}

void GameWorld::make_player_defuse_bomb(const std::string& username) {
    with_player(username, [this](Player& p) {
        if (p.is_ct() && this->can_defuse_bomb(p)) {
            p.defuse_bomb();
            this->bomb->action();
        }
    });
}

void GameWorld::stop_making_player_defuse_bomb(const std::string& username) {
    with_player(username, [this](Player& p) {
        if (p.is_ct() && p.defusing_bomb()) {
            p.stop_defusing_bomb();
            this->bomb->stop_action();
        }
    });
}

void GameWorld::equip_primary_for(const std::string& username) {
    with_player(username, [](Player& p) { p.equip_primary(); });
}

void GameWorld::equip_secondary_for(const std::string& username) {
    with_player(username, [](Player& p) { p.equip_secondary(); });
}

void GameWorld::equip_knife_for(const std::string& username) {
    with_player(username, [](Player& p) { p.equip_knife(); });
}

void GameWorld::equip_bomb_for(const std::string& username) {
    with_player(username, [](Player& p) { p.equip_bomb(); });
}

void GameWorld::pick_up_item_for(const std::string& username) {
    with_player(username, [this](Player& p) {
        Rect& player_rect = p.rect;

        std::vector<Item*> pickable_items;
        for (const auto& item: items) {
            if (player_rect.contains(item->rect)) {
                pickable_items.push_back(item.get());
            }
        }
        if (pickable_items.empty())
            return;

        Item* oldest_item = *std::min_element(
                pickable_items.begin(), pickable_items.end(),
                [](const Item* a, const Item* b) { return a->get_drop_id() < b->get_drop_id(); });

        oldest_item->try_pick_up(*this, p);
    });
}

void GameWorld::pick_up_gun_for(Player& player, DroppedGun& dropped_gun) {
    Loadout& loadout = player.get_loadout();
    std::unique_ptr<Gun> old_gun;

    if (dropped_gun.get_gun_type() == GLOCK) {
        old_gun = loadout.new_secondary_gun(dropped_gun.take_gun());
    } else {
        old_gun = loadout.new_primary_gun(dropped_gun.take_gun());
    }

    if (old_gun) {
        Rect new_rect = dropped_gun.rect;
        items.push_back(std::make_unique<DroppedGun>(new_rect, std::move(old_gun), next_drop_id++));
    }

    items.erase(std::remove_if(items.begin(), items.end(),
                               [&](const std::unique_ptr<Item>& ptr) {
                                   return ptr.get() == &dropped_gun;
                               }),
                items.end());
}

void GameWorld::try_pick_up_bomb_for(Player& player, DroppedBomb& dropped_bomb) {
    if (player.is_tt()) {
        player.receive_bomb(bomb);
        items.erase(std::remove_if(items.begin(), items.end(),
                                   [&](const std::unique_ptr<Item>& ptr) {
                                       return ptr.get() == &dropped_bomb;
                                   }),
                    items.end());
    } else {
        dropped_bomb.set_drop_id(next_drop_id++);
    }
}

void GameWorld::drop_weapons(Player& player) {
    Loadout& loadout = player.get_loadout();

    std::unique_ptr<Gun> gun = loadout.take_primary_gun();
    if (gun) {
        Rect rect(player.rect.position, ITEM_THICKNESS, ITEM_THICKNESS);
        items.push_back(std::make_unique<DroppedGun>(rect, std::move(gun), next_drop_id++));
    }

    if (loadout.has_bomb()) {
        Vector2D<int> new_pos(player.rect.position.x + player.rect.width / 2,
                              player.rect.position.y + player.rect.height / 2);
        Rect rect(new_pos, ITEM_THICKNESS, ITEM_THICKNESS);
        items.push_back(std::make_unique<DroppedBomb>(rect, next_drop_id++));
    }

    loadout.reset(true);
}

void GameWorld::buy_gun_for(const std::string& username, const GunType& gun) {
    with_player(username, [this, &gun](Player& p) {
        Loadout& loadout = p.get_loadout();

        std::unique_ptr<Gun> old_gun = this->shop.buy_gun(gun, loadout);

        if (old_gun) {
            Vector2D<int> new_pos(p.rect.position.x + p.rect.width / 2,
                                  p.rect.position.y + p.rect.height / 2);
            Rect new_rect = Rect(new_pos, ITEM_THICKNESS, ITEM_THICKNESS);
            items.push_back(
                    std::make_unique<DroppedGun>(new_rect, std::move(old_gun), next_drop_id++));
        }
    });
}

void GameWorld::buy_ammo_for(const std::string& username, const bool& for_primary) {
    with_player(username, [this, &for_primary](Player& p) {
        Loadout& loadout = p.get_loadout();

        this->shop.buy_clip(for_primary, loadout);
    });
}

const Collidable* GameWorld::colliding_object_with(const Collidable& coll) const {
    for (const auto& collidable: collidables) {
        if (collidable.get() == &coll)
            continue;
        if (collidable->collides_with(coll))
            return collidable.get();
    }
    return nullptr;
}

bool GameWorld::on_site(const Player& p) const {
    int center_x = p.rect.position.x + p.rect.width / 2;
    int center_y = p.rect.position.y + p.rect.height / 2;

    return std::any_of(sites.begin(), sites.end(), [&](const Rect& site) {
        return center_x >= site.position.x && center_x < site.position.x + site.width &&
               center_y >= site.position.y && center_y < site.position.y + site.height;
    });
}

bool GameWorld::can_defuse_bomb(const Player& player) const {
    const auto& plantation = bomb->get_plantation();
    return plantation && player.rect.intersects_with(*plantation);
}

void GameWorld::make_step_player(Player& player, const Vector2D<int>& step) {

    if (step.x != 0) {
        player.rect.position.x += step.x;
        const Collidable* colliding_obj_x = colliding_object_with(player);
        if (colliding_obj_x != nullptr) {
            const Rect& c = colliding_obj_x->rect;
            const Rect& p = player.rect;
            if (step.x > 0) {
                player.rect.position.x = c.position.x - p.width;
            } else if (step.x < 0) {
                player.rect.position.x = c.position.x + c.width;
            }
        }
    }

    if (step.y != 0) {
        player.rect.position.y += step.y;
        const Collidable* colliding_obj_y = colliding_object_with(player);
        if (colliding_obj_y != nullptr) {
            const Rect& c = colliding_obj_y->rect;
            const Rect& p = player.rect;
            if (step.y > 0) {
                player.rect.position.y = c.position.y - p.height;
            } else if (step.y < 0) {
                player.rect.position.y = c.position.y + c.height;
            }
        }
    }
}

void GameWorld::update(const float& delta_t) {
    BombStatus prev_status = bomb->get_status();

    if (prev_status == PLANTED)
        bomb->update_planted(delta_t);

    if (prev_status == PLANTED && bomb->get_status() == EXPLODED)
        make_bomb_explode();

    for (const auto& [_, player]: players) {
        if (player->is_ct() && prev_status == PLANTED && bomb->get_status() == DEFUSED &&
            player->defusing_bomb()) {
            player->stop_defusing_bomb();
        }

        player->update(*this, delta_t);
    }

    // otro for de jugadores
}


bool GameWorld::bomb_just_planted() const { return bomb->just_planted(); }
int GameWorld::bomb_detonation_time() const { return bomb->detonation_time(); }

bool GameWorld::bomb_exploded() const { return bomb->get_status() == EXPLODED; }
bool GameWorld::bomb_defused() const { return bomb->get_status() == DEFUSED; }
bool GameWorld::bomb_not_planted() const { return bomb->get_status() == NOT_PLANTED; }

void GameWorld::make_bomb_explode() {
    if (!bomb->get_plantation())
        return;
    Vector2D<int> bomb_center(
            bomb->get_plantation()->position.x + bomb->get_plantation()->width / 2,
            bomb->get_plantation()->position.y + bomb->get_plantation()->height / 2);

    for (auto& [username, player]: players) {
        if (!player->is_alive())
            continue;

        Vector2D<int> player_center(player->rect.position.x + player->rect.width / 2,
                                    player->rect.position.y + player->rect.height / 2);

        int dx = bomb_center.x - player_center.x;
        int dy = bomb_center.y - player_center.y;
        float distance = std::sqrt(static_cast<float>(dx * dx + dy * dy));

        if (distance <= bomb->get_explosion_radius()) {
            bomb->make_damage_to(*player, distance, *this);
        }
    }
}


bool GameWorld::team_is_dead(std::function<bool(const Player&)> is_in_team) const {
    return std::all_of(players.begin(), players.end(), [&](const auto& pair) {
        const Player& p = *pair.second;
        return !is_in_team(p) || !p.is_alive();
    });
}

bool GameWorld::tt_are_all_dead() const {
    return team_is_dead([](const Player& p) { return p.is_tt(); });
}

bool GameWorld::ct_are_all_dead() const {
    return team_is_dead([](const Player& p) { return p.is_ct(); });
}


void GameWorld::apply_won_round_bonus(Team team) {
    if (team == CT) {
        for (auto& [_, player]: counter_terrorists) {
            Loadout& loadout = player->get_loadout();
            loadout.add_money(Settings::getInstance().get_won_round_bonus());
        }
        for (auto& [_, player]: terrorists) {
            Loadout& loadout = player->get_loadout();
            loadout.add_money(Settings::getInstance().get_lost_round_bonus());
        }
    } else {
        for (auto& [_, player]: counter_terrorists) {
            Loadout& loadout = player->get_loadout();
            loadout.add_money(Settings::getInstance().get_lost_round_bonus());
        }
        for (auto& [_, player]: terrorists) {
            Loadout& loadout = player->get_loadout();
            loadout.add_money(Settings::getInstance().get_won_round_bonus());
        }
    }
}

GameWorld::~GameWorld() {}

std::optional<std::pair<double, Vector2D<float>>> GameWorld::impacts(
        const Shot& shot, const Collidable& collidable) const {

    Rect h = collidable.rect;

    Vector2D<float> v1(h.position.x, h.position.y);
    Vector2D<float> v2(h.position.x + h.width, h.position.y);
    Vector2D<float> v3(h.position.x + h.width, h.position.y + h.height);
    Vector2D<float> v4(h.position.x, h.position.y + h.height);

    std::vector<std::optional<std::pair<double, Vector2D<float>>>> results = {
            intersects_segment(shot, v1, v2), intersects_segment(shot, v2, v3),
            intersects_segment(shot, v3, v4), intersects_segment(shot, v4, v1)};

    std::optional<std::pair<double, Vector2D<float>>> best;

    for (const auto& result: results) {
        if (!result.has_value())
            continue;

        if (!best.has_value() || result->first < best->first)
            best = result;
    }

    return best;
}
// R(t) = origin + direction * t, con t ≥ 0 - Semirrecta por la que recorrerá el disparo.
// S(u) = seg_start + seg_dir * u, con 0 ≤ u ≤ 1 - Segmento, se quiere ver si la recta lo corta.
// Buscamos u y t para los que se cumpla: origin + direction * t  ==  seg_start + seg_dir * u
// => direction * t - seg_dir * u = seg_start - origin
// => direction * t + (-seg_dir) * u = r (siendo r = seg_start - origin)
// => ... (wolfram) =>  t = (r x (seg_dir)) / ((direction))x(seg_dir)), u = (r x direction) /
// ((shoot_direction))x(seg_dir))
std::optional<std::pair<double, Vector2D<float>>> GameWorld::intersects_segment(
        const Shot& shot, const Vector2D<float>& seg_start, const Vector2D<float>& seg_end) const {

    double orientation_in_radians = shot.orientation * M_PI / 180.0;
    Vector2D<float> direction(std::cos(orientation_in_radians), std::sin(orientation_in_radians));
    Vector2D<float> origin(shot.origin.x, shot.origin.y);

    Vector2D<float> seg_dir = seg_end - seg_start;
    Vector2D<float> r = seg_start - origin;

    double c = static_cast<double>(direction.cross(seg_dir));

    if (c == 0.0)
        return std::nullopt;  // recta y segmento son paralelos

    double t = static_cast<double>(r.cross(seg_dir)) / c;
    double u = static_cast<double>(r.cross(direction)) / c;

    if (t >= 0.0 && u >= 0.0 && u <= 1.0) {
        Vector2D<float> intersection = origin + direction * t;
        return std::make_pair(t * direction.magnitude(), intersection);
    }

    return std::nullopt;
}

void GameWorld::calculate_shot(Shot& shot, const Player& shooter) const {
    Collidable* hit = nullptr;
    std::optional<std::pair<double, Vector2D<float>>> best_impact;

    for (const auto& collidable: collidables) {
        const Collidable* coll_ptr = collidable.get();
        if (coll_ptr == &shooter)
            continue;
        if (const Player* player = dynamic_cast<const Player*>(coll_ptr)) {
            if (!player->is_alive()) {
                continue;
            }
        }

        auto impact = impacts(shot, *collidable);
        if (!impact.has_value())
            continue;

        if (!best_impact.has_value() || impact->first < best_impact->first) {
            best_impact = impact;
            hit = collidable.get();
        }
    }

    shot.hit = hit;
    shot.impact_info = best_impact;
}
