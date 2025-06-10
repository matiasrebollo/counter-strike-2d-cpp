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

Client::Client():
        skins_tt({{TerroristSkin::PHEONIX, PATH_PHEONIX_SKIN},
                  {TerroristSkin::L3337_KREW, PATH_L337_CREW_SKIN},
                  {TerroristSkin::ARTIC_AVENGER, PATH_ARTIC_SKIN},
                  {TerroristSkin::GUERRILLA, PATH_GUERRILLA_SKIN}}),
        skins_ct({{CounterTerroristSkin::SEAL_FORCE, PATH_SEAL_FORCE_SKIN},
                  {CounterTerroristSkin::GSG_9, PATH_GSG9_SKIN},
                  {CounterTerroristSkin::UK_SAS, PATH_SAS_SKIN},
                  {CounterTerroristSkin::GIGN, PATH_GIGN_SKIN}}) {}

void Client::run(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Lobby lobby;
    lobby.show();
    app.exec();

    GameUI(lobby).run();
}
