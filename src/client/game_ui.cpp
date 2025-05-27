#include "game_ui.h"

GameUI::GameUI(Lobby& lobby):
        protocol(std::move(lobby.get_protocol())),
        sender(this->protocol),
        receiver(this->protocol),
        sdl(SDLManager()),
        my_player(MyPlayer(lobby.get_username())) {
    if (!this->validate_qt_results(lobby)) {
        throw std::runtime_error("Error creating SDL interface");
    }
}

void GameUI::run() {

    GameMap map = this->receiver.receive_initial_map();

    this->sender.start();
    this->receiver.start();

    sdl.texto_prueba();

    Snapshot last_snapshot = this->receiver.receive_initial_snapshot();

    for (const PlayerDTO& p: last_snapshot.players) {
        my_player.update_my_position(p);
    }

    bool w = false, a = false, s = false, d = false;

    int it = 0;
    int FPS = 30;
    Clock clock;
    while (true) {
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
                const double angulo = my_player.calculate_angle(mouse_x, mouse_y);

                sender.add_command_to_queue(RotateDTO{angulo});
            }
        }


        sdl.clear_display();

        Snapshot snapshot_tmp;
        while (this->receiver.try_pop_snapshot_from_queue(snapshot_tmp)) {
            last_snapshot = std::move(snapshot_tmp);
        }

        sdl.render_in_z_order(map, last_snapshot, my_player);

        sdl.show_screen();

        it = clock.sleep_and_calc_next_it(FPS, it);
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
