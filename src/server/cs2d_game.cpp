#include "server/cs2d_game.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include "common/game_snapshot.h"
#include "common/game_map.h"


CS2DGame::CS2DGame() {
    const int mapWidth = 1000;
    const int mapHeight = 1000;
    const int wallThickness = 100;

    collidables.emplace_back(
            std::make_shared<Collidable>(Vector2D(0, -wallThickness), mapWidth, wallThickness));
    collidables.emplace_back(
            std::make_shared<Collidable>(Vector2D(-wallThickness, 0), wallThickness, mapHeight));
    collidables.emplace_back(
            std::make_shared<Collidable>(Vector2D(0, mapHeight), mapWidth, wallThickness));
    collidables.emplace_back(
            std::make_shared<Collidable>(Vector2D(mapWidth, 0), wallThickness, mapHeight));

    const int boxThickness = 100;

    collidables.emplace_back(
            std::make_shared<Collidable>(Vector2D(500, 500), boxThickness, boxThickness));
}

void CS2DGame::new_player(std::string& username) {  // recibir Sender/Receiver aca??
    Vector2D pos(200, 200);
    Vector2D dir(1, 0);
    auto player = std::make_shared<Player>(pos, dir);
    players[username] = player;
    collidables.push_back(player);
}

void CS2DGame::broadcast_map() const {
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

    GameMap map{objects};

    /*for (const auto& player: players) {
        player.second->send_map(map);
    }*/
}

void CS2DGame::broadcast_snapshot() const {
    std::vector<PlayerDTO> player_dtos;

    for (const auto& player: players) {
        const PlayerDTO dto{player.second->get_hitbox().position, player.second->get_direction(),
                            player.second->get_life()};
        player_dtos.push_back(dto);
    }

    const Snapshot snapshot{player_dtos};

    /*for (const auto& player: players) {
        player.second->send_snapshot(snapshot);
    }*/
}

void CS2DGame::move_player(const std::string& username, const Vector2D& direction) {
    auto it = players.find(username);
    if (it != players.end()) {
        it->second->step(direction, *this);
    } else {
        throw std::invalid_argument("Username does not correspond to a player in this game.");
    }
}

bool CS2DGame::is_player_in_valid_position(const Player& player) const {
    return std::any_of(collidables.begin(), collidables.end(),
                       [&player](const std::shared_ptr<Collidable>& collidable) {
                           return player.collides_with(*collidable);
                       });
}

void CS2DGame::rotate_player(const std::string& username, const Vector2D& direction) {
    auto it = players.find(username);
    if (it != players.end()) {
        it->second->rotate(direction);
    } else {
        throw std::invalid_argument("Username does not correspond to a player in this game.");
    }
}

double CS2DGame::impacts(const Shot& shot, const Collidable& collidable) const {
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
}

// R(t) = origin + direction * t, con t ≥ 0 - Semirrecta por la que recorrerá el disparo.
// S(u) = seg_start + seg_dir * u, con 0 ≤ u ≤ 1 - Segmento, se quiere ver si la recta lo corta.
// Buscamos u y t para los que se cumpla: origin + direction * t  ==  seg_start + seg_dir * u
// => direction * t - seg_dir * u = seg_start - origin
// => direction * t + (-seg_dir) * u = r (siendo r = seg_start - origin)
// => ... (wolfram) =>  t = (r x (seg_dir)) / ((direction))x(seg_dir)), u = (r x direction) /
// ((shoot_direction))x(seg_dir))
double CS2DGame::intersects_segment(const Shot& shot, const Vector2D& seg_start,
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
}

const Collidable* CS2DGame::first_impact(const Shot& shot) const {
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
}

void CS2DGame::shoot(const std::string& username) {
    auto it = players.find(username);
    if (it != players.end()) {
        it->second->shoot(*this);
    } else {
        throw std::invalid_argument("Username does not correspond to a player in this game.");
    }
}

void CS2DGame::run() {
    broadcast_map();

    while (should_keep_running()) {
        /*std::unique_ptr<Command> cmd;
        if (command_queue.try_pop(cmd))
            cmd->execute(*this);*/
        broadcast_snapshot();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

CS2DGame::~CS2DGame() {}
