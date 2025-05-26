#include "game_ui.h"

GameUI::GameUI(Lobby& lobby):
        protocol(std::move(lobby.get_protocol())),
        sender(this->protocol),
        receiver(this->protocol),
        username(lobby.get_username()) {
    if (!this->validate_qt_results(lobby)) {
        throw std::runtime_error("Error creating SDL interface");
    }
}

void GameUI::run() {

    GameMap map = this->receiver.receive_initial_map();

    this->sender.start();
    this->receiver.start();

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

    // Texto hardcodeado por ahora, luego hay que borrarlo
    SDL2pp::SDLTTF ttf;
    SDL2pp::Font font("../assets/gfx/fonts/sourcesans.ttf", 24);
    SDL2pp::Surface textSurface =
            font.RenderText_Solid("Waiting for players...", SDL2pp::Color(255, 255, 255));
    SDL2pp::Texture textTexture(renderer, textSurface);
    renderer.Clear();
    int textW = textSurface.GetWidth();
    int textH = textSurface.GetHeight();
    SDL2pp::Rect dstRect(210, 210, textW, textH);
    renderer.Copy(textTexture, SDL2pp::NullOpt, dstRect);
    renderer.Present();

    Snapshot last_snapshot = this->receiver.receive_initial_snapshot();

    int my_x_pos = 0, my_y_pos = 0;
    for (const PlayerDTO& p: last_snapshot.players) {
        if (p.username == this->username) {
            my_x_pos = p.position.x;
            my_y_pos = p.position.y;
            break;
        }
    }

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
                float dx = mouse_x - (float)(my_x_pos + 16);
                float dy = mouse_y - (float)(my_y_pos + 16);
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

        // auto pop_start = std::chrono::steady_clock::now();
        // int pop_count = 0;
        Snapshot snapshot_tmp;
        while (this->receiver.try_pop_snapshot_from_queue(snapshot_tmp)) {
            last_snapshot = std::move(snapshot_tmp);
            // pop_count++;
        }
        // auto pop_end = std::chrono::steady_clock::now();
        /*std::cout << "[TIMER] try_pop_snapshot: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(pop_end - pop_start)
                             .count()
                  << " us | Snapshots nuevos: " << pop_count << "\n";*/

        for (const PlayerDTO& p: last_snapshot.players) {
            if (p.username == this->username) {
                my_x_pos = p.position.x;
                my_y_pos = p.position.y;
            }
            double angulo = p.orientation;
            int x_pos = p.position.x;
            int y_pos = p.position.y;

            SDL2pp::Rect rect_origen(0, 32, 32, 32);
            SDL2pp::Rect rect_destino(x_pos, y_pos, 32, 32);
            SDL2pp::Point centro(16, 16);

            renderer.Copy(player, rect_origen, rect_destino, angulo, centro);
        }

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

bool GameUI::validate_qt_results(Lobby& lobby) {
    try {
        lobby.get_protocol();
    } catch (const std::runtime_error& e) {
        this->print_message(MSG_NO_PROTOCOL);
        return false;
    }
    if (lobby.get_username() == "") {
        this->print_message(MSG_NO_USERNAME);
        return false;
    } else if (lobby.get_gamecode() == "") {
        this->print_message(MSG_NO_GAME);
        return false;
    }
    return true;
}

void GameUI::print_message(const std::string& s) { std::cout << s << std::endl; }

GameUI::~GameUI() {
    this->sender.stop();
    this->receiver.stop();
    this->sender.close_queue();
    this->receiver.close_queue();
    this->protocol.close();
    this->sender.join();
    this->receiver.join();
}
