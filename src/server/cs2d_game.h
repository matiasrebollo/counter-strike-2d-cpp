#ifndef CS2D_GAME_H
#define CS2D_GAME_H

#include <map>
#include <memory>
#include <string>

#include "common/queue.h"
#include "common/thread.h"
#include "server/client_sender.h"
#include "server/command.h"
#include "server/game_world.h"

#define MAX_PLAYERS 10
#define MIN_PLAYERS 1
#define ROUNDS 10
#define BUY_PHASE_DURATION 10
#define ATTACK_PHASE_DURATION 120

class CS2DGame: public Thread {
private:
    std::map<std::string, std::shared_ptr<ClientSender>> players_senders;
    Queue<std::unique_ptr<Command>> command_queue;
    GameWorld game_world;
    Phase phase;
    float phase_time;
    size_t round;
    size_t last_it;

    bool should_start() const;

    void broadcast_game_dto(const GameDTO& game_dto) const;
    void broadcast_map() const;
    void broadcast_snapshot() const;

    void update(const size_t& it);
    void end_attack_phase();
    void start_phase(const Phase new_phase);
    void swap_teams();
    void end_game();

public:
    const std::string id;

    explicit CS2DGame(const std::string& id);
    bool can_add_player() const;
    void add_player(const std::string& username, std::shared_ptr<ClientSender> sender);
    void push(const std::unique_ptr<Command> command);

    void run() override;

    CS2DGame(const CS2DGame&) = delete;
    CS2DGame& operator=(const CS2DGame&) = delete;

    ~CS2DGame() override;
};

#endif
