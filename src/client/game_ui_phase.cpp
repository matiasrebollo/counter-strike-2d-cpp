#include "client/game_ui_phase.h"

#include <memory>

#include "client/game_ui.h"

GameUIPhase::GameUIPhase(GameUI& game_ui):
        game_ui(game_ui), FPS_CLIENT(Settings::getInstance().get_fps_client()) {}

void GameUIPhase::run() {
    int it = 0;
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
    if (!(game_ui.local_info.phase == ROUND_ENDED && dynamic_cast<RoundEndedPhase*>(this))) {
        change_phase();
    }
}

void GameUIPhase::change_phase() {
    if (game_ui.local_info.phase == WAITING_PLAYERS) {
        game_ui.change_phase(std::make_unique<WaitingForGamePhase>(game_ui));
    } else if (game_ui.local_info.phase == BUY) {
        game_ui.change_phase(std::make_unique<UIBuyPhase>(game_ui));
    } else if (game_ui.local_info.phase == ATTACK) {
        // el close shop hacerlo dentro del if en la fase buy en el que te fijas si cambio de clase.
        game_ui.sdl.close_shop();
        // lo mismo, cuando en buy se detecta el cambio, se llama a la funcion que reproduce el
        // sonido, no aca, o tener una variable just_started que al renderizar si es true reproduzca
        // el sonido.
        game_ui.play_start_round_sound();
        game_ui.change_phase(std::make_unique<UIAttackPhase>(game_ui));
    } else if (game_ui.local_info.phase == ROUND_ENDED) {
        // lo mismo, cuando termina fase attack, que el play_start_round_sound.
        game_ui.play_team_winner_sound();
        game_ui.change_phase(std::make_unique<RoundEndedPhase>(game_ui));
    }
    // ended ?
}

WaitingForGamePhase::WaitingForGamePhase(GameUI& game_ui): GameUIPhase(game_ui) {}
void WaitingForGamePhase::handle_game_events() { game_ui.handle_waiting_events(); }
bool WaitingForGamePhase::update_game_state() { return game_ui.update_waiting(); }
void WaitingForGamePhase::show_game(const int& it) { game_ui.show_waiting(it); }

UIBuyPhase::UIBuyPhase(GameUI& game_ui): GameUIPhase(game_ui) {}
void UIBuyPhase::handle_game_events() { game_ui.handle_buy_events(); }
bool UIBuyPhase::update_game_state() { return game_ui.update_buy(); }
void UIBuyPhase::show_game(const int& it) { game_ui.show_buy(it); }

UIAttackPhase::UIAttackPhase(GameUI& game_ui): GameUIPhase(game_ui) {}
void UIAttackPhase::handle_game_events() { game_ui.handle_attack_events(); }
bool UIAttackPhase::update_game_state() { return game_ui.update_attack(); }
void UIAttackPhase::show_game(const int& it) { game_ui.show_attack(it); }

RoundEndedPhase::RoundEndedPhase(GameUI& game_ui): GameUIPhase(game_ui) {}
void RoundEndedPhase::handle_game_events() { game_ui.handle_between_rounds_events(); }
bool RoundEndedPhase::update_game_state() { return game_ui.update_between_rounds(); }
void RoundEndedPhase::show_game(const int& it) { game_ui.show_between_rounds(it); }
