#include "client.h"

#include <QApplication>
#include <cmath>

#include "game_ui.h"

Client::Client() {}

void Client::run(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Lobby lobby;
    lobby.show();
    app.exec();

    GameUI(lobby).run();
}
