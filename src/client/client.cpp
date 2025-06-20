#include "client.h"

#include <QApplication>
#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "client/lobby/lobby.h"

#include "game_ui.h"

Client::Client() {}

void Client::run(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Lobby lobby;
    if (Settings::getInstance().get_fullscreen()) {
        lobby.showFullScreen();
    } else {
        lobby.resize(Settings::getInstance().get_window_initial_width(),
                     Settings::getInstance().get_window_initial_height());
        lobby.show();
    }
    app.exec();

    GameUI(lobby).run();
}
