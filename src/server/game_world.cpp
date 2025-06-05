#include "server/game_world.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

GameWorld::GameWorld(): spawn_zone(Vector2D(0, 0), 640, 480) {
    const int mapWidth = 640;
    const int mapHeight = 480;
    const int wallThickness = 40;

    collidables.emplace_back(std::make_shared<Collidable>(Vector2D(0, 0), mapWidth, wallThickness));
    collidables.emplace_back(
            std::make_shared<Collidable>(Vector2D(0, 0), wallThickness, mapHeight));
    collidables.emplace_back(std::make_shared<Collidable>(Vector2D(0, mapHeight - wallThickness),
                                                          mapWidth, wallThickness));
    collidables.emplace_back(std::make_shared<Collidable>(Vector2D(mapWidth - wallThickness, 0),
                                                          wallThickness, mapHeight));

    const int boxThickness = 60;

    collidables.emplace_back(std::make_shared<Collidable>(
            Vector2D((mapWidth - boxThickness) / 2, (mapHeight - boxThickness) / 2), boxThickness,
            boxThickness));
}

Vector2D GameWorld::random_position() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(spawn_zone.position.x,
                                         spawn_zone.position.x + spawn_zone.width);
    std::uniform_int_distribution<> disY(spawn_zone.position.y,
                                         spawn_zone.position.y + spawn_zone.height);
    return Vector2D(disX(gen), disY(gen));
}

void GameWorld::add_player(const std::string& username) {
    Vector2D position = random_position();
    auto player = std::make_shared<Player>(position);
    while (colliding_object_with(
            *player)) {  // mapa debe estar bien hecho como para que esto funcione
        position = random_position();
        player = std::make_shared<Player>(position);
    }
    collidables.push_back(player);

    size_t cts = counter_terrorists.size();
    size_t tts = terrorists.size();
    if (cts > tts && tts < TERRORISTS) {
        terrorists[username] = player;
    } else if (cts < COUNTER_TERRORISTS) {
        counter_terrorists[username] = player;
    }
}

const GameMap GameWorld::get_map() const {
    std::vector<MapObject> objects;
    for (const auto& collidable: collidables) {
        if (std::dynamic_pointer_cast<Player>(collidable)) {
            continue;  // ignorar jugadores
        }
        Rect h = collidable->rect;
        Vector2D pos = h.position;
        int width = h.width;
        int height = h.height;

        MapObject obj{pos, width, height, MapObjectType::BOX};
        objects.push_back(obj);
    }

    // agregar posiciones iniciales de cada jugador! (usar snapshot??)

    return GameMap{objects};
}

const GameWorldSnapshot GameWorld::get_snapshot() const {
    std::vector<PlayerDTO> ct;
    std::vector<PlayerDTO> tt;

    for (const auto& player: counter_terrorists) {
        const PlayerDTO dto{player.first, player.second->rect.position,
                            player.second->get_orientation(), player.second->get_life()};
        ct.push_back(dto);
    }
    for (const auto& player: terrorists) {
        const PlayerDTO dto{player.first, player.second->rect.position,
                            player.second->get_orientation(), player.second->get_life()};
        tt.push_back(dto);
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

void GameWorld::make_player_action(const std::string& username) {
    with_player(username, [](Player& p) { p.make_action(); });
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
    with_player(username, [&gun](Player& p) { p.buy_gun(gun); });
}

void GameWorld::buy_ammo_for(const std::string& username, const uint16_t& ammo,
                             const bool& for_primary) {
    with_player(username, [&ammo, &for_primary](Player& p) { p.buy_ammo(ammo, for_primary); });
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

void GameWorld::make_step_player(Player& player, const Vector2D& step_dir) {
    Vector2D origin = player.rect.position;
    Vector2D target = origin + step_dir * PLAYER_SPEED;
    player.rect.position = target;
    const Collidable* colliding_obj = colliding_object_with(player);
    if (colliding_obj != nullptr) {
        const Rect& c = colliding_obj->rect;
        const Rect& p = player.rect;
        Vector2D adjusted_pos = origin;
        if (step_dir.x > 0) {
            adjusted_pos.x = c.position.x - 1 - p.width;
        }
        if (step_dir.x < 0) {
            adjusted_pos.x = c.position.x + c.width + 1;
        }
        if (step_dir.y > 0) {
            adjusted_pos.y = c.position.y - 1 - p.height;
        }
        if (step_dir.y < 0) {
            adjusted_pos.y = c.position.y + c.height + 1;
        }
        player.rect.position = adjusted_pos;
    }
}

void GameWorld::update() {
    for (const auto& [_, c_terrorist]: counter_terrorists) {
        c_terrorist->update(*this);
    }
    for (const auto& [_, terrorist]: terrorists) {
        terrorist->update(*this);
    }
}

bool GameWorld::team_is_dead(const std::map<std::string, std::shared_ptr<Player>>& team) const {
    return std::all_of(team.begin(), team.end(),
                       [](const auto& player) { return !player.second->is_alive(); });
}

bool GameWorld::tt_are_all_dead() const { return team_is_dead(terrorists); }

bool GameWorld::ct_are_all_dead() const { return team_is_dead(counter_terrorists); }

GameWorld::~GameWorld() {}

/*double GameWorld::impacts(const Shot& shot, const Collidable& collidable) const {
    Rect h = collidable.rect;
    Vector2D v1 = h.position;
    Vector2D v2 = {h.position.x + h.width, h.position.y};
    Vector2D v3 = {h.position.x + h.width, h.position.y + h.height};
    Vector2D v4 = {h.position.x, h.position.y + h.height};

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
}*/

// R(t) = origin + direction * t, con t ≥ 0 - Semirrecta por la que recorrerá el disparo.
// S(u) = seg_start + seg_dir * u, con 0 ≤ u ≤ 1 - Segmento, se quiere ver si la recta lo corta.
// Buscamos u y t para los que se cumpla: origin + direction * t  ==  seg_start + seg_dir * u
// => direction * t - seg_dir * u = seg_start - origin
// => direction * t + (-seg_dir) * u = r (siendo r = seg_start - origin)
// => ... (wolfram) =>  t = (r x (seg_dir)) / ((direction))x(seg_dir)), u = (r x direction) /
// ((shoot_direction))x(seg_dir))
/*double GameWorld::intersects_segment(const Shot& shot, const Vector2D& seg_start,
                                    const Vector2D& seg_end) const {

    Vector2D seg_dir = seg_end - seg_start;
    Vector2D r = seg_start - shot.origin;

    double c = static_cast<double>(shot.direction.cross(seg_dir));

    if (c == 0)
        return 0.0;  // son paralelos, no hay intersección

    double t = static_cast<double>(r.cross(seg_dir)) / c;
    double u = static_cast<double>(r.cross(shot.direction)) / c;

    // La semirrecta solo vale para t >= 0, y el segmento para u ∈ [0,1]. Se intersecan si t y u
    // cumplen con esto.
    if (t >= 0 && u >= 0 && u <= 1) {
        return t * shot.direction.magnitude();
    }

    return 0.0;
}*/

/*const Collidable* GameWorld::first_impact(const Shot& shot) const {
    const Collidable* hit = nullptr;
    double closest = std::numeric_limits<double>::max();

    for (const auto& collidable: collidables) {
        double dist = impacts(shot, *collidable);
        if (dist != 0.0) {
            if (dist < closest) {
                closest = dist;
                hit = collidable.get();
            }
        }
    }

    return hit;
}*/

/*void GameWorld::shoot(const std::string& username) {
    auto it = players.find(username);
    if (it != players.end()) {
        it->second->shoot(*this);
    } else {
        throw std::invalid_argument("Username does not correspond to a player in this game.");
    }
}*/
