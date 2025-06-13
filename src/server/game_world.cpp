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

GameWorld::GameWorld(const std::string& map_filename):
        shop(), game_map(YamlParser().yaml_to_game_map(PATH_FOLDER_MAPS + map_filename + ".yaml")) {
    add_collidables();
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
    std::cout << "Agregando a gameworld: " + username << std::endl;
    Vector2D<int> default_position(-100, -100);
    auto player = std::make_shared<Player>(username, default_position);
    collidables.push_back(player);

    size_t cts = counter_terrorists.size();
    size_t tts = terrorists.size();
    if (cts < COUNTER_TERRORISTS) {
        counter_terrorists[username] = player;
    } else if (tts < TERRORISTS) {
        terrorists[username] = player;
    } else {
        throw std::runtime_error("No hay lugar para más jugadores");
    }
}

void GameWorld::restart_players() {
    for (auto& [_, player]: terrorists) {
        player->restart();
    }
    for (auto& [_, player]: counter_terrorists) {
        player->restart();
    }
}

void GameWorld::spawn_players() {
    for (auto& [_, player]: terrorists) {
        Vector2D<int> position = random_tt_spawn_position();
        player->rect.position = position;

        while (colliding_object_with(*player)) {
            position = random_tt_spawn_position();
            player->rect.position = position;
        }
    }

    for (auto& [_, player]: counter_terrorists) {
        Vector2D<int> position = random_ct_spawn_position();
        player->rect.position = position;

        while (colliding_object_with(*player)) {
            position = random_ct_spawn_position();
            player->rect.position = position;
        }
    }
}

const GameMap GameWorld::get_map() const { return this->game_map; }

const ShopInfoDTO GameWorld::get_shop_info() const { return this->shop.get_shop_info(); }

const GameWorldSnapshot GameWorld::get_snapshot() const {
    std::vector<PlayerDTO> ct;
    std::vector<PlayerDTO> tt;

    for (const auto& player: counter_terrorists) {
        ct.push_back(player.second->get_dto());
    }

    for (const auto& player: terrorists) {
        tt.push_back(player.second->get_dto());
    }

    return GameWorldSnapshot{ct, tt};
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
    with_player(username, [](Player& p) { p.make_action(); });
}

void GameWorld::stop_making_player_action(const std::string& username) {
    with_player(username, [](Player& p) { p.stop_making_action(); });
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

void GameWorld::buy_gun_for(const std::string& username, const GunType& gun) {
    with_player(username, [this, &gun](Player& p) {
        Loadout& loadout = p.get_loadout();

        this->shop.buy_gun(gun, loadout);
        // std::unique_ptr<Gun> old_gun = this->shop.buy_gun(gun, loadout);

        /*if (old_gun) {
            this->drop_gun(p.get_position(), old_gun); // o similar
        }*/
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
        if (coll.collides_with(*collidable))
            return collidable.get();
    }
    return nullptr;
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
    for (const auto& [_, c_terrorist]: counter_terrorists) {
        c_terrorist->update(*this, delta_t);
    }
    for (const auto& [_, terrorist]: terrorists) {
        terrorist->update(*this, delta_t);
    }
}

bool GameWorld::team_is_dead(const std::map<std::string, std::shared_ptr<Player>>& team) const {
    return std::all_of(team.begin(), team.end(),
                       [](const auto& player) { return !player.second->is_alive(); });
}

bool GameWorld::tt_are_all_dead() const { return team_is_dead(terrorists); }

bool GameWorld::ct_are_all_dead() const { return team_is_dead(counter_terrorists); }

GameWorld::~GameWorld() {}

double GameWorld::impacts(const Shot& shot, const Collidable& collidable) const {
    Rect h = collidable.rect;
    Vector2D<float> v1(static_cast<float>(h.position.x), static_cast<float>(h.position.y));
    Vector2D<float> v2(static_cast<float>(h.position.x + h.width),
                       static_cast<float>(h.position.y));
    Vector2D<float> v3(static_cast<float>(h.position.x + h.width),
                       static_cast<float>(h.position.y + h.height));
    Vector2D<float> v4(static_cast<float>(h.position.x),
                       static_cast<float>(h.position.y + h.height));

    std::vector<double> distances = {
            intersects_segment(shot, v1, v2), intersects_segment(shot, v2, v3),
            intersects_segment(shot, v3, v4), intersects_segment(shot, v4, v1)};

    auto it = std::min_element(distances.begin(), distances.end(), [](double a, double b) {
        if (a == 0.0)
            return false;
        if (b == 0.0)
            return true;
        return a < b;
    });

    return (it != distances.end() && *it > 0.0) ? *it : 0.0;
}

// R(t) = origin + direction * t, con t ≥ 0 - Semirrecta por la que recorrerá el disparo.
// S(u) = seg_start + seg_dir * u, con 0 ≤ u ≤ 1 - Segmento, se quiere ver si la recta lo corta.
// Buscamos u y t para los que se cumpla: origin + direction * t  ==  seg_start + seg_dir * u
// => direction * t - seg_dir * u = seg_start - origin
// => direction * t + (-seg_dir) * u = r (siendo r = seg_start - origin)
// => ... (wolfram) =>  t = (r x (seg_dir)) / ((direction))x(seg_dir)), u = (r x direction) /
// ((shoot_direction))x(seg_dir))
double GameWorld::intersects_segment(const Shot& shot, const Vector2D<float>& seg_start,
                                     const Vector2D<float>& seg_end) const {
    double real_orientation = shot.orientation - 90.0f;

    double orientation_in_radians = real_orientation * M_PI / 180.0;
    Vector2D<float> direction(std::cos(orientation_in_radians), std::sin(orientation_in_radians));
    Vector2D<float> origin(shot.origin.x, shot.origin.y);

    Vector2D<float> seg_dir = seg_end - seg_start;
    Vector2D<float> r = seg_start - origin;

    double c = static_cast<double>(direction.cross(seg_dir));

    if (c == 0)
        return 0.0;  // son paralelos, no hay intersección

    double t = static_cast<double>(r.cross(seg_dir)) / c;
    double u = static_cast<double>(r.cross(direction)) / c;

    // La semirrecta solo vale para t >= 0, y el segmento para u ∈ [0,1]. Se intersecan si t y u
    // cumplen con esto.
    if (t >= 0 && u >= 0 && u <= 1) {
        return t * direction.magnitude();
    }

    return 0.0;
}

void GameWorld::calculate_shot(Shot& shot, const Player& shooter) const {
    Collidable* hit = nullptr;
    double closest = std::numeric_limits<double>::max();

    for (const auto& collidable: collidables) {
        if (collidable.get() == &shooter)
            continue;
        double dist = impacts(shot, *collidable);
        if (dist != 0.0) {
            if (dist < closest) {
                closest = dist;
                hit = collidable.get();
            }
        }
    }

    shot.hit = hit;
    shot.distance = closest;
}
