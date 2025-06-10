#include "game_ui.h"

#include <variant>

#include <unistd.h>

#include "common/game_dto.h"

GameUI::GameUI(Lobby& lobby):
        protocol(std::move(lobby.get_protocol())),
        sdl(SDLManager()),
        input_handler(sdl, this->protocol),
        receiver(this->protocol),
        // podria usar move?
        local_info{lobby.get_username(), lobby.get_gamecode(), lobby.get_ct_skin(),
                   lobby.get_tt_skin()},
        keep_running(true) {
    if (!this->validate_qt_results(lobby)) {
        throw std::runtime_error(
                "Error creating SDL interface");  // quizas ponerlo en los get de lobby.
    }
    this->phase = std::make_unique<WaitingForGamePhase>(*this);
}

void GameUI::run() {
    input_handler.start_sender();
    this->receiver.start();
    try {
        while (this->keep_running) {
            phase->run();
        }
    } catch (const ClosedQueue& e) {
        std::cout << "The server has been closed!" << std::endl;
        this->keep_running = false;
    }

    this->handle_game_ended();
    this->close_client();
}

void GameUI::update_local_info_from_snapshot(const Snapshot& snapshot) {
    for (const auto& p: snapshot.ct) {
        if (p.username == local_info.username) {
            local_info.is_ct = true;
            local_info.life = p.life;
            local_info.x = p.position.x;
            local_info.y = p.position.y;
            local_info.money = p.loadout.money;
            return;
        }
    }

    for (const auto& p: snapshot.tt) {
        if (p.username == local_info.username) {
            local_info.is_ct = false;
            local_info.life = p.life;
            local_info.x = p.position.x;
            local_info.y = p.position.y;
            local_info.money = p.loadout.money;
            return;
        }
    }
}

void GameUI::handle_waiting_events() { this->keep_running = input_handler.handle_waiting_events(); }
bool GameUI::update_waiting() {
    GameDTO game_dto;
    bool pop = true;
    while (pop) {
        if (!this->receiver.try_pop_game_dto(game_dto)) {
            pop = false;
            continue;
        }
        std::visit(
                [this, &pop](const auto& game_dto) {
                    using T = std::decay_t<decltype(game_dto)>;
                    if constexpr (std::is_same_v<T, Snapshot>) {
                        this->game_snapshot = std::move(game_dto);
                        update_local_info_from_snapshot(this->game_snapshot);
                    } else if constexpr (std::is_same_v<T, GameMap>) {
                        this->map = std::move(game_dto);  // guardarlo en sdl??
                    } else if constexpr (std::is_same_v<T, GameEnded>) {
                        // guardar estadisticas
                        // estado ended?
                        this->keep_running = false;
                    }
                },
                game_dto);
        if (this->game_snapshot.phase != WAITING_PLAYERS)
            return false;
    }
    return true;
}

void GameUI::show_waiting(const int& it) {
    sdl.clear_display();
    // el 2 luego tiene que ser la cantidad de personas que va a unirse maxima
    sdl.render_waiting_screen(this->game_snapshot.ct.size() + this->game_snapshot.tt.size(), 2,
                              local_info.gamename, it, FPS);
    sdl.show_screen();
}

void GameUI::handle_buy_events() { this->keep_running = input_handler.handle_buy_events(); }
bool GameUI::update_buy() {
    GameDTO game_dto;
    bool pop = true;
    while (pop) {
        if (!this->receiver.try_pop_game_dto(game_dto)) {
            pop = false;
            continue;
        }
        Snapshot snapshot_tmp = std::get<Snapshot>(game_dto);
        // Identificar en snapshot_tmp cambios de equipamiento en el local_player para animación de
        // tienda
        this->game_snapshot = std::move(snapshot_tmp);
        update_local_info_from_snapshot(this->game_snapshot);

        if (this->game_snapshot.phase != BUY) {
            return false;
        }
    }
    return true;
}

void GameUI::show_buy(const int& /*it*/) {
    sdl.clear_display();
    sdl.render_in_z_order(this->map, this->game_snapshot, local_info);
    sdl.render_shop(local_info.money);
    sdl.render_crosshair(this->game_snapshot, local_info);
    sdl.show_screen();
}

void GameUI::handle_attack_events() { this->keep_running = input_handler.handle_events(); }
bool GameUI::update_attack() {
    GameDTO game_dto;
    bool pop = true;
    while (pop) {
        if (!this->receiver.try_pop_game_dto(game_dto)) {
            pop = false;
            continue;
        }
        if (std::holds_alternative<GameEnded>(game_dto)) {
            this->keep_running = false;
            pop = false;
            continue;
        }

        Snapshot snapshot_tmp = std::get<Snapshot>(game_dto);
        // Identificar en snapshot_tmp cambios/eventos para activar animaciones
        this->game_snapshot = std::move(snapshot_tmp);
        update_local_info_from_snapshot(this->game_snapshot);

        if (snapshot_tmp.phase != ATTACK) {
            return false;
        }
    }
    return true;
}
void GameUI::show_attack(const int& /*it*/) {
    sdl.clear_display();
    sdl.render_in_z_order(this->map, this->game_snapshot, local_info);
    sdl.render_crosshair(this->game_snapshot, local_info);
    sdl.show_screen();
}

/*void GameUI::update() { idea para sacar codigo repetido de la actualizacion del juego popeando
snapshots GameDTO game_dto; bool pop = true; while (pop) { if
(!this->receiver.try_pop_game_dto(game_dto)) { pop = false; continue;
        }
        handle_x_game_dto(game_dto); // x = segun fase
        if (std::holds_alternative<GameEnded>(game_dto)) {
            keep_running = false;
            pop = false;
            continue;
        }

        Snapshot snapshot_tmp = std::get<Snapshot>(game_dto);
        // Identificar en snapshot_tmp cambios/eventos para activar animaciones
        this->game_snapshot = std::move(snapshot_tmp);
        // chequear keep running de fase!
    }
}*/

void GameUI::handle_game_ended() { std::cout << "Game ended!" << std::endl; }

void GameUI::change_phase(std::unique_ptr<GameUIPhase> new_phase) {
    this->phase = std::move(new_phase);
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
