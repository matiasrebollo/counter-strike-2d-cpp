#ifndef GAME_UI_H
#define GAME_UI_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../common/clock.h"
#include "lobby/lobby.h"

#include "SDLManager.h"
#include "client_protocol.h"
#include "client_receiver.h"
#include "input_handler.h"
#include "my_player.h"

#define MSG_NO_PROTOCOL "You have to connect yourself to a server to play :)"
#define MSG_NO_USERNAME "You have to enter your username to login in our server"
#define MSG_NO_GAME "You have to create a game or join one to play!"
#define FPS 30

class GameUI {
private:
    ClientProtocol protocol;
    SDLManager sdl;
    InputHandler input_handler;
    ClientReceiver receiver;
    MyPlayer my_player;

    bool validate_qt_results(Lobby& lobby);
    void print_message(const std::string& s);

public:
    explicit GameUI(Lobby& lobby);
    void run();
    ~GameUI();
};

#endif
