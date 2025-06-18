#ifndef GAME_UI_H
#define GAME_UI_H

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../common/clock.h"
#include "../common/skins.h"
#include "common/settings.h"
#include "lobby/lobby.h"

#include "SDLManager.h"
#include "client_protocol.h"
#include "client_receiver.h"
#include "game_ui_phase.h"
#include "input_handler.h"
#include "local_info.h"

class GameUI {
private:
    ClientProtocol&& protocol;
    SDLManager sdl;
    InputHandler input_handler;
    // estaria bueno quizas englobar el receiver en una clase que reciba y procese la snapshot
    ClientReceiver receiver;
    LocalInfo local_info;
    std::unique_ptr<GameUIPhase> phase;
    bool keep_running;
    bool just_planted = false;
    bool make_sound_planted = false;
    bool just_defuse = false;
    bool make_sound_defused = false;
    bool make_sound_clock = false;

    friend class GameUIPhase;
    friend class WaitingForGamePhase;
    friend class UIBuyPhase;
    friend class UIAttackPhase;
    friend class RoundEndedPhase;
    friend class GameEndedPhase;

    /* Setea en false todos los eventos de cada player */
    void reset_player_events();

    /* Detecta eventos para cada player en una snapshot*/
    void detect_player_events(const Snapshot& snapshot);

    /* Actualiza la informacion local del estado del juego en base a una snapshot
     */
    void update_local_info_from_snapshot(const Snapshot& snapshot);

    /* Maneja eventos de usuario en la fase waiting */
    void handle_waiting_events();
    /* Recibe snapshots del receiver hasta recibir el mapa y cambiar de fase */
    bool update_waiting();
    /* Renderiza la fase waiting */
    void show_waiting(const int& it);

    /* Maneja eventos de usuario en la fase de compra */
    void handle_buy_events();
    /* Recibe snapshots del receiver hasta cambiar de fase */
    bool update_buy();
    /* Renderiza la fase de compra */
    void show_buy(const int& it);

    /* Maneja eventos de usuario en la fase de juego */
    void handle_attack_events();
    /* Recibe snapshots del receiver hasta cambiar de fase */
    bool update_attack();
    /* Renderiza la fase de juego */
    void show_attack(const int& it);

    void handle_between_rounds_events();
    bool update_between_rounds();
    void show_between_rounds(const int& it);

    void change_phase(std::unique_ptr<GameUIPhase> new_phase);

    void handle_game_ended();
    void close_client();

    void play_start_round_sound();

    void play_team_winner_sound();

    void update_game_status(const Snapshot& snapshot);

public:
    explicit GameUI(Lobby& lobby);
    void run();

    ~GameUI();
};

#endif
