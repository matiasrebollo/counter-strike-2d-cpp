#include "server/game_phase.h"

#include <utility>

#include "common/clock.h"
#include "server/cs2d_game.h"

GamePhase::GamePhase(CS2DGame& game, const size_t& duration): game(game), duration(duration) {}

void GamePhase::run() {
    Clock clock;
    size_t last_it = 0;
    size_t it = 0;
    float time = 0.0f;

    while (should_continue() && time < duration) {
        size_t delta_it = it - last_it;
        float delta_seconds = static_cast<float>(delta_it) / FPS;
        time += delta_seconds;

        std::unique_ptr<Command> cmd;
        while (game.command_queue.try_pop(cmd)) {
            execute(std::move(cmd));
        }
        game.broadcast_snapshot();
        game.update(it, last_it);

        last_it = it;
        it = clock.sleep_and_calc_next_it(FPS, it);
    }

    end();
}

WaitingPlayersPhase::WaitingPlayersPhase(CS2DGame& game):
        GamePhase(game, WAITING_PLAYERS_PHASE_DURATION) {}
Phase WaitingPlayersPhase::type() { return WAITING_PLAYERS; }
bool WaitingPlayersPhase::should_continue() { return !game.should_start(); }
void WaitingPlayersPhase::execute(std::unique_ptr<Command>) {}
void WaitingPlayersPhase::end() {
    game.broadcast_map();
    game.change_phase(std::make_unique<BuyPhase>(game));
    // y si salió porque terminó la partida???
}


BuyPhase::BuyPhase(CS2DGame& game): GamePhase(game, BUY_PHASE_DURATION) {}
Phase BuyPhase::type() { return BUY; }
bool BuyPhase::should_continue() { return true; }
void BuyPhase::execute(std::unique_ptr<Command> cmd) { game.execute_in_buy_phase(std::move(cmd)); }
void BuyPhase::end() { game.change_phase(std::make_unique<AttackPhase>(game)); }

AttackPhase::AttackPhase(CS2DGame& game): GamePhase(game, ATTACK_PHASE_DURATION) {}
Phase AttackPhase::type() { return ATTACK; }
bool AttackPhase::should_continue() {
    return true;  // poner condiciones de fin de fase!!
}
void AttackPhase::execute(std::unique_ptr<Command> cmd) {
    game.execute_in_attack_phase(std::move(cmd));
}
void AttackPhase::end() {
    game.end_attack_phase();
    game.change_phase(std::make_unique<BuyPhase>(game));
    // fase entre rondas?? si termina la partida???
}
