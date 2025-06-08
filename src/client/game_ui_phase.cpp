#include "client/game_ui_phase.h"

#include <memory>

#include "client/game_ui.h"

GameUIPhase::GameUIPhase(GameUI& game_ui): game_ui(game_ui) {}

void GameUIPhase::run() {
    int it = 0;
    Clock clock;
    while (true) {
        show_game(it);  // pasar la iteracion para animaciones??
        handle_game_events();
        if (!game_ui.keep_running)
            break;
        update_game_state();  // identificar cambios en el juego y detectar eventos. LANZAR
                              // animaciones que se veran en show_game.
        if (!keep_running() || !game_ui.keep_running)
            break;

        it = clock.sleep_and_calc_next_it(FPS, it);
    }

    change_phase();
}

void GameUIPhase::change_phase() {
    if (game_ui.game_snapshot.phase == WAITING_PLAYERS) {
        game_ui.change_phase(std::make_unique<WaitingForGamePhase>(game_ui));
    } else if (game_ui.game_snapshot.phase == BUY) {
        game_ui.change_phase(std::make_unique<UIBuyPhase>(game_ui));
    } else if (game_ui.game_snapshot.phase == ATTACK) {
        game_ui.change_phase(std::make_unique<UIAttackPhase>(game_ui));
    }
    // ended ?
}

WaitingForGamePhase::WaitingForGamePhase(GameUI& game_ui): GameUIPhase(game_ui) {}
void WaitingForGamePhase::handle_game_events() { game_ui.handle_waiting_events(); }
void WaitingForGamePhase::update_game_state() { game_ui.update_waiting(); }
bool WaitingForGamePhase::keep_running() { return game_ui.game_snapshot.phase == WAITING_PLAYERS; }
void WaitingForGamePhase::show_game(const int& it) { game_ui.show_waiting(it); }


UIBuyPhase::UIBuyPhase(GameUI& game_ui): GameUIPhase(game_ui) {}
void UIBuyPhase::handle_game_events() { game_ui.handle_buy_events(); }
void UIBuyPhase::update_game_state() { game_ui.update_buy(); }
bool UIBuyPhase::keep_running() { return game_ui.game_snapshot.phase == BUY; }
void UIBuyPhase::show_game(const int& it) { game_ui.show_buy(it); }

UIAttackPhase::UIAttackPhase(GameUI& game_ui): GameUIPhase(game_ui) {}
void UIAttackPhase::handle_game_events() { game_ui.handle_attack_events(); }
void UIAttackPhase::update_game_state() { game_ui.update_attack(); }
bool UIAttackPhase::keep_running() { return game_ui.game_snapshot.phase == ATTACK; }
void UIAttackPhase::show_game(const int& it) { game_ui.show_attack(it); }

/*GameEndedPhase::GameEndedPhase(GameUI& game_ui): GameUIPhase(game_ui) {}
void GameEndedPhase::handle_game_events() {

}
void GameEndedPhase::update_game_state() {

}
bool GameEndedPhase::keep_running() {

}
void GameEndedPhase::show_game() {

}*/
