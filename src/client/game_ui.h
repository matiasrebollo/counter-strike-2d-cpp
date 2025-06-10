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
#include "game_ui_phase.h"
#include "input_handler.h"
#include "local_info.h"

#define MSG_NO_PROTOCOL "You have to connect yourself to a server to play :)"
#define BASH_MSG_NO_USERNAME "You have to enter your username to login in our server"
#define MSG_NO_GAME "You have to create a game or join one to play!"
#define FPS 30

class GameUI {
private:
    ClientProtocol protocol;
    SDLManager sdl;
    InputHandler input_handler;
    // estaria bueno quizas englobar el receiver en una clase que reciba y procese la snapshot
    ClientReceiver receiver;
    LocalInfo local_info;
    std::unique_ptr<GameUIPhase> phase;
    bool keep_running;
    GameMap map;  // guardarlo en sdl.
    Snapshot
            game_snapshot;  // deberia ser un objeto propio de game_ui, no el dto para comunicacion!

    friend class GameUIPhase;
    friend class WaitingForGamePhase;
    friend class UIBuyPhase;
    friend class UIAttackPhase;
    friend class GameEndedPhase;

    bool validate_qt_results(Lobby& lobby);
    void print_message(const std::string& s);
    void update_local_info_from_snapshot(const Snapshot& snapshot);

    void handle_waiting_events();
    bool update_waiting();
    void show_waiting(const int& it);

    void handle_buy_events();
    bool update_buy();
    void show_buy(const int& it);

    void handle_attack_events();
    bool update_attack();
    void show_attack(const int& it);

    void change_phase(std::unique_ptr<GameUIPhase> new_phase);

    void handle_game_ended();
    void close_client();

public:
    explicit GameUI(Lobby& lobby);
    void run();
    void handle_buy_phase(const GameMap& map);
    void handle_attack_phase(const GameMap& map);

    ~GameUI();
};

#endif
