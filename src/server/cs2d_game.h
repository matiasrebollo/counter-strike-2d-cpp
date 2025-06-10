#ifndef CS2D_GAME_H
#define CS2D_GAME_H

#include <map>
#include <memory>
#include <optional>
#include <string>

#include "common/queue.h"
#include "common/settings.h"
#include "common/thread.h"
#include "server/client_sender.h"
#include "server/command.h"
#include "server/game_phase.h"
#include "server/game_world.h"

class CS2DGame: public Thread {
private:
    std::map<std::string, std::shared_ptr<ClientSender>> players_senders;
    Queue<std::unique_ptr<Command>> command_queue;

    GameWorld game_world;
    std::unique_ptr<GamePhase> phase;
    size_t current_round;
    std::optional<Team> current_round_winner;
    size_t ct_wins;
    size_t tt_wins;

    friend class GamePhase;
    friend class WaitingPlayersPhase;
    friend class BuyPhase;
    friend class AttackPhase;
    friend class BetweenRoundsPhase;


    bool should_start() const;

    void broadcast_game_dto(const GameDTO& game_dto) const;
    void broadcast_map() const;
    void broadcast_snapshot(const int time_left) const;

    bool current_round_has_a_winner() const;
    void decide_winner();
    void begin_new_round();
    void swap_teams();
    void change_phase(std::unique_ptr<GamePhase> new_phase);
    void update(const float& delta_t);
    void execute_in_attack_phase(std::unique_ptr<Command> cmd);
    void execute_in_buy_phase(std::unique_ptr<Command> cmd);
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
