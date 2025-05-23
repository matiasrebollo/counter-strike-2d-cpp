#include "client.h"

#include <QApplication>
#include <cmath>
#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/clock.h"
#include "client/lobby/lobby.h"

#include "client_receiver.h"
#include "client_sender.h"

Client::Client() {}

void Client::run(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Lobby lobby;
    lobby.show();
    app.exec();

    // std::string username = lobby.get_username();

    ClientSender sender = ClientSender(lobby.get_protocol());
    ClientReceiver receiver = ClientReceiver(lobby.get_protocol());

    GameMap map = receiver.receive_initial_map();

    sender.start();
    receiver.start();

    SDL2pp::SDL sdl(SDL_INIT_VIDEO);
    SDL2pp::Window window("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                          SDL_WINDOW_RESIZABLE);
    SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Jugador
    SDL2pp::Surface playerSheet("../assets/gfx/player/ct1.bmp");
    SDL2pp::Texture player(renderer, playerSheet);

    // Caja
    SDL2pp::Surface boxSheet("../assets/gfx/tiles/aztec.bmp");
    SDL2pp::Texture box(renderer, boxSheet);

    float x_pos = 100;
    float y_pos = 100;

    int it = 0;
    int FPS = 30;
    Clock clock;
    while (true) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return;
            /*if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return;
                    case SDLK_w:
                        sender.add_command_to_queue(MoveUpDTO{});
                        break;
                    case SDLK_a:
                        sender.add_command_to_queue(MoveLeftDTO{});
                        break;
                    case SDLK_s:
                        sender.add_command_to_queue(MoveDownDTO{});
                        break;
                    case SDLK_d:
                        sender.add_command_to_queue(MoveRightDTO{});
                        break;
                }
            }*/
            if (event.type == SDL_MOUSEMOTION) {
                int mouse_x = event.motion.x;
                int mouse_y = event.motion.y;
                float dx = mouse_x - x_pos;
                float dy = mouse_y - y_pos;
                float ang_radianes = atan2(dy, dx);
                const double angulo = (ang_radianes * 180.0f / M_PI) + 90;
                sender.add_command_to_queue(RotateDTO{angulo});
            }
        }

        Snapshot snapshot = receiver.pop_snapshot_from_queue();
        PlayerDTO p = snapshot.players[0];
        double angulo = p.orientation;

        renderer.Clear();

        // ACA SI ITERO EL MAPA (POR AHORA SOLO TIPO BOX)
        for (const MapObject& obj: map.map_objects) {
            if (obj.type == MapObjectType::BOX) {
                SDL2pp::Rect rect_origen(416, 64, 32, 32);  // por ahora lo hardcodeo
                SDL2pp::Rect rect_destino(obj.position.x, obj.position.y, obj.width, obj.height);
                renderer.Copy(box, rect_origen, rect_destino);
            }
        }

        // podria tambien crear los rect y point antes en lugar de en el copy
        renderer.Copy(player, SDL2pp::Rect(0, 32, 32, 32),
                      SDL2pp::Rect(p.position.x, p.position.y, 32, 32), angulo,
                      SDL2pp::Point(16.0f, 16.0f));
        renderer.Present();


        it = clock.sleep_and_calc_next_it(FPS, it);
    }
}
