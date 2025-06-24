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
    ClientReceiver receiver;
    LocalInfo local_info;
    std::unique_ptr<GameUIPhase> phase;
    bool keep_running;

    friend class GameUIPhase;
    friend class WaitingForGamePhase;
    friend class UIBuyPhase;
    friend class UIAttackPhase;
    friend class RoundEndedPhase;

    /* Setea en false todos los eventos */
    void reset_events();

    /* Detecta eventos en una snapshot*/
    void detect_events(const Snapshot& snapshot);

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

    bool game_has_ended();

    void update_player(const PlayerDTO& player, const bool& is_ct);

public:
    explicit GameUI(Lobby& lobby);
    void run();

    ~GameUI();
};

#endif
