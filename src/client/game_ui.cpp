#include "game_ui.h"

#include <variant>

#include <unistd.h>

#include "common/game_dto.h"

GameUI::GameUI(Lobby& lobby):
        protocol(std::move(lobby.get_protocol())),
        sdl(SDLManager()),
        input_handler(sdl, this->protocol),
        receiver(this->protocol),
        // podria usar move.
        local_player_info{lobby.get_username(), lobby.get_gamecode(), lobby.get_ct_skin(),
                          lobby.get_tt_skin()},
        state(std::make_unique<WaitingForGameState>()),
        keep_running(true) {
    if (!this->validate_qt_results(lobby)) {
        throw std::runtime_error(
                "Error creating SDL interface");  // quizas ponerlo en los get de lobby.
    }
}

void GameUI::run() {
    input_handler.start_sender();
    this->receiver.start();
    try {
        while (this->keep_running) {
            state->handle(*this);
        }
    } catch (const ClosedQueue& e) {
        std::cout << "The server has been closed!" << std::endl;
        this->keep_running = false;
    }

    this->close_client();
}

void GameUI::handle_waiting_phase() {
    Snapshot last_snapshot;

    int it = 0;
    Clock clock;
    bool loop_waiting = true;
    while (loop_waiting) {
        loop_waiting = input_handler.handle_waiting_events();
        if (!loop_waiting) {
            keep_running = false;
            break;
        }

        GameDTO game_dto;
        bool pop = true;
        while (pop) {
            if (!this->receiver.try_pop_game_dto(game_dto)) {
                pop = false; 
                continue; // es como  break
            }
            process_waiting(game_dto, last_snapshot, loop_waiting, pop);
        }

        sdl.clear_display();
        // el 2 luego tiene que ser la cantidad de personas que va a unirse maxima
        sdl.render_waiting_screen(last_snapshot.ct.size() + last_snapshot.tt.size(), 2,
                                  local_player_info.gamename, it, FPS);
        sdl.show_screen();
        it = clock.sleep_and_calc_next_it(FPS, it);
    }
}
void GameUI::handle_buy_phase(const GameMap& map) {
    Snapshot last_snapshot = std::get<Snapshot>(this->receiver.pop_game_dto());

    int it = 0;
    Clock clock;
    bool loop_buy = true;
    while (loop_buy) {
        loop_buy = input_handler.handle_buy_events();
        if (!loop_buy) {
            keep_running = false;
            break;
        }

        GameDTO snapshot_tmp;
        bool pop = true;
        while (pop) {
            if (!this->receiver.try_pop_game_dto(snapshot_tmp)) {
                pop = false;
                continue;
            }
            Snapshot snapshot = std::get<Snapshot>(snapshot_tmp);
            if (snapshot.phase == Phase::ATTACK) {
                loop_buy = false;
                pop = false;
                continue;
            }
            last_snapshot = std::move(snapshot);
        }

        sdl.clear_display();

        sdl.render_in_z_order(map, last_snapshot, local_player_info);
        sdl.render_shop();

        sdl.show_screen();

        it = clock.sleep_and_calc_next_it(FPS, it);
    }
    this->change_state(std::make_unique<AttackPhaseState>(std::move(map)));
}

void GameUI::handle_attack_phase(const GameMap& map) {
    // esto no deberia ser bloqueante, si no hay ninguna, deberia dibujar el ultimo snapshot de la
    // fase buy.
    Snapshot last_snapshot = std::get<Snapshot>(this->receiver.pop_game_dto());

    int it = 0;
    Clock clock;
    bool loop_game = true;
    while (loop_game) {
        loop_game = input_handler.handle_events();
        if (!loop_game) {
            keep_running = false;
            break;
        }

        GameDTO snapshot_tmp;
        /*while (this->receiver.try_pop_game_dto(snapshot_tmp)) {
            if (std::holds_alternative<GameEnded>(snapshot_tmp)) {
                this->state = std::make_unique<GameEndedState>();
                return;
            }
            last_snapshot = std::move(std::get<Snapshot>(snapshot_tmp));
        }*/
        bool pop = true;
        while (pop) {
            if (!this->receiver.try_pop_game_dto(snapshot_tmp)) {
                pop = false;
                continue;
            }


            Snapshot snapshot = std::get<Snapshot>(snapshot_tmp);
            if (snapshot.phase == Phase::BUY) {
                loop_game = false;
                pop = false;
                continue;
            }

            last_snapshot = std::move(snapshot);
        }

        sdl.clear_display();
        sdl.render_in_z_order(map, last_snapshot, local_player_info);
        sdl.show_screen();

        it = clock.sleep_and_calc_next_it(FPS, it);
    }

    this->change_state(std::make_unique<BuyPhaseState>(std::move(map)));
}

void GameUI::handle_game_ended_phase() {
    std::cout << "Game ended!" << std::endl;
    this->keep_running = false;
}

void GameUI::change_state(std::unique_ptr<GameUIState> new_state) {
    this->state = std::move(new_state);
}

void GameUI::process_waiting(GameDTO& dto, Snapshot& snapshot, bool& loop, bool& pop) {
    std::visit(
            [this, &snapshot, &loop, &pop](const auto& game_dto) {
                using T = std::decay_t<decltype(game_dto)>;
                if constexpr (std::is_same_v<T, Snapshot>) {
                    snapshot = std::move(game_dto);
                } else if constexpr (std::is_same_v<T, GameMap>) {
                    this->change_state(std::make_unique<AttackPhaseState>(std::move(game_dto)));
                    loop = false;
                    pop = false;
                } else if constexpr (std::is_same_v<T, GameEnded>) {
                    this->change_state(std::make_unique<GameEndedState>());
                    loop = false;
                    pop = false;
                }
            },
            dto);
}

bool GameUI::validate_qt_results(Lobby& lobby) {
    try {
        lobby.get_protocol();
    } catch (const std::runtime_error& e) {
        this->print_message(MSG_NO_PROTOCOL);
        return false;
    }
    if (lobby.get_username() == "") {
        this->print_message(BASH_MSG_NO_USERNAME);
        return false;
    } else if (lobby.get_gamecode() == "") {
        this->print_message(MSG_NO_GAME);
        return false;
    }
    return true;
}

void GameUI::print_message(const std::string& s) { std::cout << s << std::endl; }

void GameUI::close_client() {
    this->protocol.close();
    // this->receiver.close_queue();
    this->receiver.join();
    // El receiver ya no me interesa, cerro su queue y ya está.
    // this->input_handler.close_sender_queue();
    this->input_handler.close_sender_queue();
    this->input_handler
            .join_sender();  // aca me bloqueo hasta que sea joineable, por dentro el sender stopea
}

GameUI::~GameUI() {
    if (this->keep_running) {
        this->close_client();
    }
}
