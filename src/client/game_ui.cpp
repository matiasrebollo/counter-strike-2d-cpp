#include "game_ui.h"

#include <variant>

#include "common/game_dto.h"

GameUI::GameUI(Lobby& lobby):
        protocol(std::move(lobby.get_protocol())),
        sdl(SDLManager()),
        input_handler(sdl, this->protocol),
        receiver(this->protocol),
        username(lobby.get_username()),
        gamename(lobby.get_gamecode()),
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

    while (this->keep_running) {
        state->handle(*this);  // en lugar de mandar aca, guardarlo como atributo
    }
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
                pop = false;  // break
                continue;
            }
            process_waiting(game_dto, last_snapshot, loop_waiting, pop);
        }

        sdl.clear_display();
        sdl.render_waiting_screen(last_snapshot.ct.size() + last_snapshot.tt.size(), 2, gamename,
                                  it, FPS);
        sdl.show_screen();
        it = clock.sleep_and_calc_next_it(FPS, it);
    }
}
void GameUI::handle_buy_phase(const GameMap& /*map*/) {}
void GameUI::handle_attack_phase(const GameMap& map) {
    Snapshot last_snapshot =
            std::get<Snapshot>(this->receiver.pop_game_dto());  // esto despues lo recibe el mapa

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
            last_snapshot = std::move(std::get<Snapshot>(snapshot_tmp));
        }


        sdl.clear_display();

        sdl.render_in_z_order(map, last_snapshot, this->username);

        sdl.show_screen();

        it = clock.sleep_and_calc_next_it(FPS, it);
    }
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

GameUI::~GameUI() {
    receiver.close_queue();
    receiver.stop();
    receiver.join();
    // El receiver ya no me interesa, cerro su queue y ya está.
    input_handler.close_sender_queue();
    input_handler
            .join_sender();  // aca me bloqueo hasta que sea joineable, por dentro el sender stopea
    protocol.close();
}
