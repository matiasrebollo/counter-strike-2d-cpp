#include "game_ui.h"

#include <variant>

#include <unistd.h>

#include "common/game_dto.h"

GameUI::GameUI(Lobby& lobby):
        protocol(std::move(lobby.get_protocol())),
        sdl(SDLManager()),
        input_handler(sdl, this->protocol),
        receiver(this->protocol),
        my_player(MyPlayer(lobby.get_username())),
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
        std::cout << "SERVER CLOSED!" << std::endl;
        this->keep_running = false;
    }

    this->close_client();
}

void GameUI::handle_waiting_for_game() {
    Snapshot last_snapshot;

    int it = 0;
    Clock clock;
    bool loop_game = true;
    while (loop_game) {
        loop_game = input_handler.handle_waiting_events();
        if (!loop_game) {
            keep_running = false;
            break;
        }

        GameDTO game_dto;
        bool pop = true;
        while (pop) {
            if (!this->receiver.try_pop_game_dto(game_dto)) {
                pop = false;
                continue;
            }

            std::visit(
                    [this, &last_snapshot, &loop_game, &pop](const auto& game_dto) {
                        using T = std::decay_t<decltype(game_dto)>;
                        if constexpr (std::is_same_v<T, Snapshot>) {
                            last_snapshot = std::move(game_dto);
                        } else if constexpr (std::is_same_v<T, GameMap>) {
                            state = std::make_unique<AttackPhaseState>(
                                    game_dto);  // deberia usarse buy phase
                            pop = false;
                            loop_game = false;
                        } else if constexpr (std::is_same_v<T, GameEnded>) {
                            state = std::make_unique<GameEndedState>();
                            pop = false;
                            loop_game = false;
                        } else {
                            static_assert(always_false_v<T>, "Unhandled GameDTO type");
                        }
                    },
                    game_dto);
        }

        sdl.texto_prueba();
        // hacer algo con la snapshot??
        // actualizar el cartel de esperando players!!!

        it = clock.sleep_and_calc_next_it(FPS, it);
    }
}
void GameUI::handle_buy_phase(const GameMap& /*map*/) {}
void GameUI::handle_attack_phase(const GameMap& map) {
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
        while (this->receiver.try_pop_game_dto(snapshot_tmp)) {
            if (std::holds_alternative<GameEnded>(snapshot_tmp)) {
                this->state = std::make_unique<GameEndedState>();
                return;
            }
            last_snapshot = std::move(std::get<Snapshot>(snapshot_tmp));
        }

        std::vector<std::vector<PlayerDTO>> teams = {last_snapshot.ct, last_snapshot.tt};

        for (const std::vector<PlayerDTO>& team: teams) {
            for (const PlayerDTO& p: team) {
                my_player.update_my_position(p);
            }
        }

        sdl.clear_display();

        sdl.render_in_z_order(map, last_snapshot, my_player.get_username());

        sdl.show_screen();

        it = clock.sleep_and_calc_next_it(FPS, it);
    }
}

void GameUI::handle_game_ended_phase() {
    std::cout << "Game ended!" << std::endl;
    this->keep_running = false;
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
