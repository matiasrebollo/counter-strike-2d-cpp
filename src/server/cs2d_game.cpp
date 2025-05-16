#include "server/cs2d_game.h"

#include <iostream>
#include <string>
#include <utility>

CS2DGame::CS2DGame() {
    const int mapWidth = 1000;
    const int mapHeight = 1000;
    const int wallThickness = 100;

    collidables.emplace_back(
            std::make_shared<Collidable>(Vector2D(0, -wallThickness), mapWidth, wallThickness));
    collidables.emplace_back(
            std::make_shared<Collidable>(Vector2D(-wallThickness, 0), wallThickness, mapHeight));
    collidables.emplace_back(
            std::make_shared<Collidable>(Vector2D(0, mapHeight), mapWidth, wallThickness));
    collidables.emplace_back(
            std::make_shared<Collidable>(Vector2D(mapWidth, 0), wallThickness, mapHeight));

    const int boxThickness = 100;

    collidables.emplace_back(
            std::make_shared<Collidable>(Vector2D(500, 500), boxThickness, boxThickness));
}

void CS2DGame::new_player(std::string& username) {  // recibir Sender/Receiver aca??
    auto player = std::make_shared<Player>(Vector2D(200, 200), Vector2D(1, 0));
    players[username] = player;
    collidables.push_back(player);
}

void CS2DGame::broadcast_map() const {
    // crear struct que corresponda.
    // recorrer Players y enviar struct a cada uno
}

void CS2DGame::broadcast_snapshot() const {
    // crear struct que corresponda.
    // recorrer Players y enviar struct a cada uno
}

void CS2DGame::run() {
    broadcast_map();

    while (should_keep_running()) {
        // std::unique_ptr<Command> cmd;
        /*if (command_queue.try_pop(cmd)) {
            cmd->execute(*this);


        }*/
        broadcast_snapshot();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

CS2DGame::~CS2DGame() {}
