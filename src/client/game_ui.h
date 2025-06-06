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
#include "my_player.h"

#define MSG_NO_PROTOCOL "You have to connect yourself to a server to play :)"
#define BASH_MSG_NO_USERNAME "You have to enter your username to login in our server"
#define MSG_NO_GAME "You have to create a game or join one to play!"
#define FPS 30

class GameUI {
private:
    ClientProtocol protocol;
    SDLManager sdl;
    InputHandler input_handler;
    ClientReceiver receiver;
    MyPlayer my_player;
    std::unique_ptr<GameUIState> state;
    bool keep_running;

    bool validate_qt_results(Lobby& lobby);
    void print_message(const std::string& s);
    void close_client();

public:
    explicit GameUI(Lobby& lobby);
    void run();
    void handle_waiting_for_game();
    void handle_buy_phase(const GameMap& map);
    void handle_attack_phase(const GameMap& map);
    void handle_game_ended_phase();
    ~GameUI();
};

#endif
