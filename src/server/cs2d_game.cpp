#include "server/cs2d_game.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "common/clock.h"
#include "common/game_map.h"
#include "common/game_snapshot.h"

CS2DGame::CS2DGame(const std::string& id):
        spawn_zone(Vector2D(0, 0), 640, 480),
        phase(BUY),
        phase_time(0.0f),
        round(0),
        last_it(0),
        id(id) {
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

bool CS2DGame::can_add_player() const { return players.size() < MAX_PLAYERS; }

bool CS2DGame::should_start() const {
    if (players.size() < MIN_PLAYERS)
        return false;

    if (players.size() > players_senders.size())
        return false;

    return true;
}

Vector2D CS2DGame::random_position() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(spawn_zone.position.x,
                                         spawn_zone.position.x + spawn_zone.width);
    std::uniform_int_distribution<> disY(spawn_zone.position.y,
                                         spawn_zone.position.y + spawn_zone.height);
    return Vector2D(disX(gen), disY(gen));
}

void CS2DGame::add_player(const std::string& username) {
    double orientation = 0.0;
    Vector2D position = random_position();
    auto player = std::make_shared<Player>(position, orientation);
    while (is_player_not_in_valid_position(*player)) {
        position = random_position();
        player = std::make_shared<Player>(position, orientation);
    }

    collidables.push_back(player);
    players[username] = player;
}

void CS2DGame::add_player_sender(const std::string& username,
                                 std::shared_ptr<ClientSender> sender) {
    if (players.find(username) == players.end()) {
        throw std::runtime_error("Cannot add sender: player with username '" + username +
                                 "' does not exist.");
    }

    std::vector<MapObject> objects;
    for (const auto& collidable: collidables) {
        if (std::dynamic_pointer_cast<Player>(collidable)) {
            continue;  // ignorar jugadores
        }
        Rect h = collidable->get_rect();
        Vector2D pos = h.position;
        int width = h.width;
        int height = h.height;

        MapObject obj{pos, width, height, MapObjectType::BOX};
        objects.push_back(obj);
    }
    const GameMap map{objects};
    sender->send_map(map);

    players_senders[username] = sender;
    if (this->should_start())
        this->start();
}

void CS2DGame::push(std::unique_ptr<Command> command) { command_queue.push(std::move(command)); }

void CS2DGame::broadcast_snapshot() const {
    std::vector<PlayerDTO> player_dtos;

    for (const auto& player: players) {
        const PlayerDTO dto{player.first, player.second->get_rect().position,
                            player.second->get_orientation(), player.second->get_life()};
        player_dtos.push_back(dto);
    }

    const Snapshot snapshot{player_dtos};

    for (const auto& [_, sender]: players_senders) {
        sender->push(snapshot);
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

void CS2DGame::update(const size_t& it) {
    for (size_t i = 0; i < it - this->last_it; ++i) {
        // actualizar cosas propias del juego
        for (const auto& [_, player]: players) {
            player->update(*this);
        }
    }
    this->last_it = it;
}

void CS2DGame::end_attack_phase() {
    // limpiar items del mapa (dejar algunos)
    // reiniciar posiciones de cada jugador al spawn
    // ver que equipo ganó!
}

void CS2DGame::start_phase(const Phase new_phase) {
    if (new_phase == BUY)
        this->round += 1;
    this->phase = new_phase;
    this->last_it = 0;
    this->phase_time = 0.0f;
}

void CS2DGame::swap_teams() {
    // cambiar de equipos
    // cambiar skins a cada jugador (segun las que seleccionó)
}

void CS2DGame::end_game() {
    // finalizar partida (llamar a stop())
    // determinar equipo ganador y enviar stadisticas finales
}

void CS2DGame::run() {
    if (!should_start()) {
        throw(std::runtime_error("Cannot start game: not all players are ready."));
    }

    int FPS = 60;
    Clock clock;
    size_t it = 1;
    broadcast_snapshot();
    while (should_keep_running()) {
        if (this->round > ROUNDS)
            end_game();
        if (this->round == ROUNDS / 2)
            swap_teams();
        size_t delta_it = it - last_it;
        float delta_seconds = static_cast<float>(delta_it) / FPS;
        this->phase_time += delta_seconds;
        if (this->phase == BUY) {
            std::unique_ptr<Command> cmd;
            while (command_queue.try_pop(cmd)) {
                if (cmd->type() == BuyPhase)
                    cmd->execute(*this);
            }
            this->last_it = it;
            std::cout << this->phase_time << std::endl;
            if (this->phase_time >= BUY_PHASE_DURATION) {
                start_phase(ATTACK);
                it = 1;
            }
        } else {
            std::unique_ptr<Command> cmd;
            while (command_queue.try_pop(cmd)) {
                if (cmd->type() == AttackPhase)
                    cmd->execute(*this);
            }
            update(it);
            if (this->phase_time >= ATTACK_PHASE_DURATION) {
                end_attack_phase();
                start_phase(BUY);
                it = 1;
            }
        }
        broadcast_snapshot();
        // std::cout << "last it: " << it << std::endl;
        it = clock.sleep_and_calc_next_it(FPS, it);
        // std::cout << "new it: " << it << std::endl;
    }
}

CS2DGame::~CS2DGame() {}

/*double CS2DGame::impacts(const Shot& shot, const Collidable& collidable) const {
    Rect h = collidable.get_Rect();
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
