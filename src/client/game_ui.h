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
#include "lobby/lobby.h"

#include "SDLManager.h"
#include "client_protocol.h"
#include "client_receiver.h"
#include "game_ui_state.h"
#include "input_handler.h"
#include "local_player_info.h"

#define FPS 30

class GameUI {
private:
    ClientProtocol protocol;
    SDLManager sdl;
    InputHandler input_handler;
    ClientReceiver receiver;
    LocalPlayerInfo local_player_info;
    std::unique_ptr<GameUIState> state;
    bool keep_running;

    void close_client();
    void process_waiting(GameDTO& dto, Snapshot& snapshot, bool& loop, bool& pop);
    void change_state(std::unique_ptr<GameUIState> new_state);

public:
    explicit GameUI(Lobby& lobby);
    void run();
    void handle_waiting_phase();
    void handle_buy_phase(const GameMap& map);
    void handle_attack_phase(const GameMap& map);
    void handle_game_ended_phase();
    ~GameUI();
};

#endif
