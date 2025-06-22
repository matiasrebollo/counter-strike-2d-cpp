#include "client/game_ui_phase.h"

#include <memory>

#include "client/game_ui.h"

GameUIPhase::GameUIPhase(GameUI& game_ui, int starting_it): game_ui(game_ui), it(starting_it) {}

void GameUIPhase::run() {
    Clock clock;
    bool loop = true;

    while (loop) {
        show_game(it);  // pasar la iteracion para animaciones
        handle_game_events();
        if (!game_ui.keep_running)
            break;
        if (!update_game_state()) {
            loop = false;  // hubo cambio de fase
        }                  // identificar cambios en el juego y detectar eventos. LANZAR
                           // animaciones que se veran en show_game.
        // luego habria que sacarlo
        if (!game_ui.keep_running)
            break;
        it = clock.sleep_and_calc_next_it(FPS_CLIENT, it);
    }
    change_phase(it);
}

void GameUIPhase::change_phase(int last_it) {
    if (game_ui.local_info.phase == WAITING_PLAYERS) {
        game_ui.change_phase(std::make_unique<WaitingForGamePhase>(game_ui, 0));
    } else if (game_ui.local_info.phase == BUY) {
        game_ui.change_phase(std::make_unique<UIBuyPhase>(game_ui, 0));
    } else if (game_ui.local_info.phase == ATTACK) {
        game_ui.change_phase(std::make_unique<UIAttackPhase>(game_ui, last_it));
    } else if (game_ui.local_info.phase == ROUND_ENDED) {
        game_ui.change_phase(std::make_unique<RoundEndedPhase>(game_ui, last_it));
    }
    // ended ?
}

WaitingForGamePhase::WaitingForGamePhase(GameUI& game_ui, int starting_it):
        GameUIPhase(game_ui, starting_it) {}
void WaitingForGamePhase::handle_game_events() { game_ui.handle_waiting_events(); }
bool WaitingForGamePhase::update_game_state() { return game_ui.update_waiting(); }
void WaitingForGamePhase::show_game(const int& it) { game_ui.show_waiting(it); }


UIBuyPhase::UIBuyPhase(GameUI& game_ui, int starting_it): GameUIPhase(game_ui, starting_it) {}
void UIBuyPhase::handle_game_events() { game_ui.handle_buy_events(); }
bool UIBuyPhase::update_game_state() { return game_ui.update_buy(); }
void UIBuyPhase::show_game(const int& it) { game_ui.show_buy(it); }

UIAttackPhase::UIAttackPhase(GameUI& game_ui, int starting_it): GameUIPhase(game_ui, starting_it) {}
void UIAttackPhase::handle_game_events() { game_ui.handle_attack_events(); }
bool UIAttackPhase::update_game_state() { return game_ui.update_attack(); }
void UIAttackPhase::show_game(const int& it) { game_ui.show_attack(it); }

RoundEndedPhase::RoundEndedPhase(GameUI& game_ui, int starting_it):
        GameUIPhase(game_ui, starting_it) {}
void RoundEndedPhase::handle_game_events() { game_ui.handle_between_rounds_events(); }
bool RoundEndedPhase::update_game_state() { return game_ui.update_between_rounds(); }
void RoundEndedPhase::show_game(const int& it) { game_ui.show_between_rounds(it); }

/*GameEndedPhase::GameEndedPhase(GameUI& game_ui): GameUIPhase(game_ui) {}
void GameEndedPhase::handle_game_events() {

}
void GameEndedPhase::update_game_state() {

}
bool GameEndedPhase::keep_running() {

}
void GameEndedPhase::show_game() {

}*/
