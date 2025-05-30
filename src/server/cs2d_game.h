#ifndef CS2D_GAME_H
#define CS2D_GAME_H

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>

#include "common/game_map.h"
#include "common/game_snapshot.h"
#include "common/message.h"
#include "common/queue.h"
#include "common/thread.h"
#include "server/client_sender.h"
#include "server/collidable.h"
#include "server/command.h"
#include "server/player.h"
// #include "server/shot.h"

#define MAX_PLAYERS 10
#define MIN_PLAYERS 2
#define BUY_PHASE_DURATION 20
#define ATTACK_PHASE_DURATION 60
#define ROUNDS 10

class CS2DGame: public Thread {
private:
    std::map<std::string, std::shared_ptr<Player>> players;
    std::map<std::string, std::shared_ptr<ClientSender>> players_senders;
    std::map<std::string, std::shared_ptr<Queue<Snapshot>>> player_queues;
    std::list<std::shared_ptr<Collidable>> collidables;
    const Rect spawn_zone;
    Queue<std::unique_ptr<Command>> command_queue;
    Phase phase;
    float phase_time;
    size_t round;
    size_t last_it;

    bool should_start() const;
    Vector2D random_position() const;
    Vector2D spawn_position() const;

    void broadcast_map() const;
    void broadcast_snapshot() const;

    template <typename PlayerAction>
    void with_player(const std::string& username, PlayerAction action) {
        auto it = players.find(username);
        if (it != players.end()) {
            action(*it->second);
        } else {
            throw std::invalid_argument("Username does not correspond to a player in this game.");
        }
    }

    void update(const size_t& it);

    // devuelve la distancia del objeto con el que impactó o 0 si no impactó.
    // double impacts(const Shot& shot, const Collidable& collidable) const;
    // double intersects_segment(const Shot& shot, const Vector2D& seg_start, const Vector2D&
    // seg_end) const;

    void start_phase(const Phase new_phase);
    void end_attack_phase();
    void swap_teams();
    void end_game();

public:
    const std::string id;

    explicit CS2DGame(const std::string& id);
    bool can_add_player() const;
    void add_player(const std::string& username, std::shared_ptr<ClientSender> sender);
    void push(const std::unique_ptr<Command> command);
    void rotate_player(const std::string& username, const double& angle);
    void move_player_up(const std::string& username);
    void move_player_down(const std::string& username);
    void move_player_left(const std::string& username);
    void move_player_right(const std::string& username);
    bool is_player_not_in_valid_position(const Player& player) const;
    // void shoot(const std::string& username);
    // const Collidable* first_impact(const Shot& shot) const;

    void run() override;

    CS2DGame(const CS2DGame&) = delete;
    CS2DGame& operator=(const CS2DGame&) = delete;

    ~CS2DGame() override;
};

#endif
