#include "server/cs2d_game.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include "common/clock.h"
#include "common/game_map.h"
#include "common/game_snapshot.h"

CS2DGame::CS2DGame(const std::string& id): last_it(0), id(id) {
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

void CS2DGame::new_player(const std::string& username, ClientSender& sender) {
    Vector2D position(100, 100);
    double orientation = 0.0;
    auto player = std::make_shared<Player>(position, orientation, sender);

    std::vector<MapObject> objects;
    for (const auto& collidable: collidables) {
        if (std::dynamic_pointer_cast<Player>(collidable)) {
            continue;  // ignorar jugadores
        }
        Hitbox h = collidable->get_hitbox();
        Vector2D pos = h.position;
        int width = h.width;
        int height = h.height;

        MapObject obj{pos, width, height, MapObjectType::BOX};
        objects.push_back(obj);
    }
    const GameMap map{objects};

    player->send_map(map);
    players[username] = player;
    collidables.push_back(player);
}

void CS2DGame::push(std::unique_ptr<Command> command) { command_queue.push(std::move(command)); }

void CS2DGame::broadcast_snapshot() {
    std::vector<PlayerDTO> player_dtos;

    for (const auto& player: players) {
        const PlayerDTO dto{player.first, player.second->get_hitbox().position,
                            player.second->get_orientation(), player.second->get_life()};
        player_dtos.push_back(dto);
    }

    const Snapshot snapshot{player_dtos};

    for (const auto& player: players) {
        player.second->send_snapshot(snapshot);
    }
}

void CS2DGame::rotate_player(const std::string& username, const double& angle) {
    with_player(username, [angle](Player& p) { p.rotate(angle); });
}

void CS2DGame::move_player_up(const std::string& username) {
    with_player(username, [](Player& p) { p.move_up(); });
}

void CS2DGame::move_player_down(const std::string& username) {
    with_player(username, [](Player& p) { p.move_down(); });
}

void CS2DGame::move_player_left(const std::string& username) {
    with_player(username, [](Player& p) { p.move_left(); });
}

void CS2DGame::move_player_right(const std::string& username) {
    with_player(username, [](Player& p) { p.move_right(); });
}


bool CS2DGame::is_player_not_in_valid_position(const Player& player) const {
    return std::any_of(collidables.begin(), collidables.end(),
                       [&player](const std::shared_ptr<Collidable>& collidable) {
                           if (collidable.get() == &player)
                               return false;
                           return player.collides_with(*collidable);
                       });
}

/*double CS2DGame::impacts(const Shot& shot, const Collidable& collidable) const {
    Hitbox h = collidable.get_hitbox();
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
/*double CS2DGame::intersects_segment(const Shot& shot, const Vector2D& seg_start,
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

/*const Collidable* CS2DGame::first_impact(const Shot& shot) const {
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

/*void CS2DGame::shoot(const std::string& username) {
    auto it = players.find(username);
    if (it != players.end()) {
        it->second->shoot(*this);
    } else {
        throw std::invalid_argument("Username does not correspond to a player in this game.");
    }
}*/

void CS2DGame::update(const size_t& it) {
    for (size_t i = 0; i < it - this->last_it; ++i) {
        // actualizar cosas propias del juego
        for (const auto& [_, player]: players) {
            player->update(*this);
        }
    }
    this->last_it = it;
}

void CS2DGame::run() {
    int FPS = 60;
    Clock clock;
    size_t it = 1;
    while (should_keep_running()) {
        std::unique_ptr<Command> cmd;
        if (command_queue.try_pop(cmd))
            cmd->execute(*this);
        update(it);
        broadcast_snapshot();
        // std::cout << "last it: " << it << std::endl;
        it = clock.sleep_and_calc_next_it(FPS, it);
        // std::cout << "new it: " << it << std::endl;
    }
}

CS2DGame::~CS2DGame() {}
