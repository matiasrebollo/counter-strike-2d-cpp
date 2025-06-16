#include "server/game_phase.h"

#include <cmath>
#include <utility>

#include "common/clock.h"
#include "server/cs2d_game.h"

GamePhase::GamePhase(CS2DGame& game, const size_t& duration): game(game), duration(duration) {}

void GamePhase::run() {
    Clock clock;
    size_t last_it = 0;
    size_t it = 0;
    float time = 0.0f;

    Settings& settings = Settings::getInstance();
    int FPS_SERVER = settings.get_fps_server();

    while (should_continue() && time < duration) {
        size_t delta_it = it - last_it;
        float delta_seconds = static_cast<float>(delta_it) / FPS_SERVER;
        time += delta_seconds;
        game.broadcast_snapshot(duration - std::trunc(time));
        if (!game.should_keep_running())
            return;
        std::unique_ptr<Command> cmd;
        while (game.command_queue.try_pop(cmd)) {
            execute(std::move(cmd));
        }
        if (type() != WAITING_PLAYERS)
            game.update(delta_seconds);

        last_it = it;
        it = clock.sleep_and_calc_next_it(FPS_SERVER, it);
    }

    end();
}

WaitingPlayersPhase::WaitingPlayersPhase(CS2DGame& game):
        GamePhase(game, Settings::getInstance().get_waiting_phase_duration()) {}
Phase WaitingPlayersPhase::type() { return WAITING_PLAYERS; }
bool WaitingPlayersPhase::should_continue() {
    return !game.should_start() && game.should_keep_running();
}
void WaitingPlayersPhase::execute(std::unique_ptr<Command>) {}
void WaitingPlayersPhase::end() {
    game.broadcast_game_initial_info();
    game.begin_new_round();
    game.change_phase(std::make_unique<BuyPhase>(game));
    // y si salió porque terminó la partida??? -> AGREGAR
}


BuyPhase::BuyPhase(CS2DGame& game):
        GamePhase(game, Settings::getInstance().get_buy_phase_duration()) {}
Phase BuyPhase::type() { return BUY; }
bool BuyPhase::should_continue() { return game.should_keep_running(); }
void BuyPhase::execute(std::unique_ptr<Command> cmd) { game.execute_in_buy_phase(std::move(cmd)); }
void BuyPhase::end() { game.change_phase(std::make_unique<AttackPhase>(game)); }

AttackPhase::AttackPhase(CS2DGame& game):
        GamePhase(game, Settings::getInstance().get_attack_phase_duration()) {}
Phase AttackPhase::type() { return ATTACK; }
bool AttackPhase::should_continue() {
    return !game.current_round_has_a_winner() && game.should_keep_running();
}
void AttackPhase::execute(std::unique_ptr<Command> cmd) {
    game.execute_in_attack_phase(std::move(cmd));
}
void AttackPhase::end() {
    game.decide_winner();
    game.change_phase(std::make_unique<BetweenRoundsPhase>(game));
    // si termina la partida???
}

BetweenRoundsPhase::BetweenRoundsPhase(CS2DGame& game):
        GamePhase(game, Settings::getInstance().get_between_rounds_phase_duration()) {}
Phase BetweenRoundsPhase::type() { return ATTACK; }
bool BetweenRoundsPhase::should_continue() { return game.should_keep_running(); }
void BetweenRoundsPhase::execute(std::unique_ptr<Command> cmd) {
    game.execute_in_attack_phase(std::move(cmd));
}
void BetweenRoundsPhase::end() {
    game.begin_new_round();
    game.change_phase(std::make_unique<BuyPhase>(game));
    // si termina la partida???
}
