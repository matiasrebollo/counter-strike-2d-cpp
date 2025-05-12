#include "server/cs2d_game.h"

#include <iostream>
#include <string>
#include <utility>

CS2DGame::CS2DGame(): players() {
    const float mapWidth = 10.5f;
    const float mapHeight = 10.5f;
    const float wallThickness = 1.0f;

    map_objects.emplace_back(Vector2D{0, 0 - wallThickness}, mapWidth, wallThickness);
    map_objects.emplace_back(Vector2D{0 - wallThickness, 0}, wallThickness, mapHeight);
    map_objects.emplace_back(Vector2D{0, mapHeight}, mapWidth, wallThickness);
    map_objects.emplace_back(Vector2D{mapWidth, 0}, wallThickness, mapHeight);

    const float boxThickness = 1.0f;

    map_objects.emplace_back(Vector2D{5, 5}, boxThickness, boxThickness);
}

void CS2DGame::add_player() {
    Player player({0.5f, 0.5f});
    players.emplace_back(std::move(player));
}

void CS2DGame::game_loop() {
    std::string input;

    std::cout << "Comandos: w (arriba), s (abajo), a (izquierda), d (derecha), q (salir)\n";

    print_map_objects();

    while (running) {
        players[0].print_position();

        std::cout << "> ";
        std::cin >> input;

        Vector2D dir{0, 0};

        if (input == "w") {
            dir.y = -1;
        } else if (input == "s") {
            dir.y = 1;
        } else if (input == "a") {
            dir.x = -1;
        } else if (input == "d") {
            dir.x = 1;
        } else if (input == "q") {
            running = false;
            continue;
        } else {
            std::cout << "Comando inválido.\n";
            continue;
        }

        players[0].step(dir, map_objects);
    }

    std::cout << "Juego terminado.\n";
}

void CS2DGame::print_map_objects() const {
    for (const auto& obj: map_objects) {
        Hitbox hitbox = obj.get_hitbox();  // Obtenemos el hitbox

        std::cout << "Objeto en posición (" << hitbox.position.x << ", " << hitbox.position.y
                  << "), Ancho: " << hitbox.width << ", Alto: " << hitbox.height << "\n";
    }
}

CS2DGame::~CS2DGame() {}
