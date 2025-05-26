#include "client.h"

#include <QApplication>
#include <chrono>  // al principio del archivo
#include <cmath>
#include <iostream>
#include <utility>
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

    Snapshot last_snapshot = receiver.receive_initial_snapshot();
    const PlayerDTO& p_inicial = last_snapshot.players[0];
    int x_pos = p_inicial.position.x;
    int y_pos = p_inicial.position.y;

    bool w = false, a = false, s = false, d = false;


    int it = 0;
    int FPS = 30;
    Clock clock;
    while (true) {
        // auto frame_start = std::chrono::steady_clock::now();  // INICIO DEL FRAME
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return;
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return;
                    case SDLK_w:
                        if (!w) {
                            sender.add_command_to_queue(MoveUpDTO{});
                            w = true;
                        }
                        break;
                    case SDLK_a:
                        if (!a) {
                            sender.add_command_to_queue(MoveLeftDTO{});
                            a = true;
                        }
                        break;
                    case SDLK_s:
                        if (!s) {
                            sender.add_command_to_queue(MoveDownDTO{});
                            s = true;
                        }
                        break;
                    case SDLK_d:
                        if (!d) {
                            sender.add_command_to_queue(MoveRightDTO{});
                            d = true;
                        }
                        break;
                }
            }
            if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        if (w) {
                            sender.add_command_to_queue(MoveUpDTO{});
                            w = false;
                        }
                        break;
                    case SDLK_a:
                        if (a) {
                            sender.add_command_to_queue(MoveLeftDTO{});
                            a = false;
                        }
                        break;
                    case SDLK_s:
                        if (s) {
                            sender.add_command_to_queue(MoveDownDTO{});
                            s = false;
                        }
                        break;
                    case SDLK_d:
                        if (d) {
                            sender.add_command_to_queue(MoveRightDTO{});
                            d = false;
                        }
                        break;
                }
            }
            if (event.type == SDL_MOUSEMOTION) {
                int mouse_x = event.motion.x;
                int mouse_y = event.motion.y;
                float dx = mouse_x - (float)(x_pos + 16);
                float dy = mouse_y - (float)(y_pos + 16);
                float ang_radianes = atan2(dy, dx);
                const double angulo = (ang_radianes * 180.0f / M_PI) + 90;

                // auto send_start = std::chrono::steady_clock::now();
                sender.add_command_to_queue(RotateDTO{angulo});
                // auto send_end = std::chrono::steady_clock::now();
                /*std::cout << "[TIMER] Envío comando: "
                          << std::chrono::duration_cast<std::chrono::microseconds>(send_end -
                                                                                   send_start)
                                     .count()
                          << " us\n";*/
            }
        }

        // auto pop_start = std::chrono::steady_clock::now();
        // int pop_count = 0;
        Snapshot snapshot_tmp;
        while (receiver.try_pop_snapshot_from_queue(snapshot_tmp)) {
            last_snapshot = std::move(snapshot_tmp);
            // pop_count++;
        }
        // auto pop_end = std::chrono::steady_clock::now();
        /*std::cout << "[TIMER] try_pop_snapshot: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(pop_end - pop_start)
                             .count()
                  << " us | Snapshots nuevos: " << pop_count << "\n";*/

        const PlayerDTO& p = last_snapshot.players[0];
        double angulo = p.orientation;
        x_pos = p.position.x;
        y_pos = p.position.y;

        std::cout << "Posicion x: " << x_pos << std::endl;
        std::cout << "Posicion y: " << y_pos << std::endl;
        std::cout << "\n";

        // auto render_start = std::chrono::steady_clock::now();
        renderer.Clear();

        // ACA SI ITERO EL MAPA (POR AHORA SOLO TIPO BOX)
        for (const MapObject& obj: map.map_objects) {
            if (obj.type == MapObjectType::BOX) {
                SDL2pp::Rect rect_origen(416, 64, 32, 32);  // por ahora lo hardcodeo
                std::cout << "Posicion caja x: " << obj.position.x << std::endl;
                std::cout << "Posicion caja y: " << obj.position.y << std::endl;
                std::cout << "Caja ancho: " << obj.width << std::endl;
                std::cout << "Caja alto: " << obj.height << std::endl;
                SDL2pp::Rect rect_destino(obj.position.x, obj.position.y, obj.width, obj.height);
                renderer.Copy(box, rect_origen, rect_destino);
            }
        }

        // podria tambien crear los rect y point antes en lugar de en el copy
        renderer.Copy(player, SDL2pp::Rect(0, 32, 32, 32), SDL2pp::Rect(x_pos, y_pos, 32, 32),
                      angulo, SDL2pp::Point(16, 16));

        renderer.Present();

        // auto render_end = std::chrono::steady_clock::now();
        /*std::cout << "[TIMER] Render: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(render_end -
                                                                           render_start)
                             .count()
                  << " us\n";*/

        // auto sleep_start = std::chrono::steady_clock::now();
        it = clock.sleep_and_calc_next_it(FPS, it);
        // auto sleep_end = std::chrono::steady_clock::now();
        /*std::cout << "[TIMER] Sleep: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(sleep_end - sleep_start)
                             .count()
                  << " us\n";*/

        // auto frame_end = std::chrono::steady_clock::now();
        /*std::cout << "[TIMER] Frame completo: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(frame_end - frame_start)
                             .count()
                  << " ms\n\n";*/
    }
}
