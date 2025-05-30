#include "game_ui.h"

GameUI::GameUI(Lobby& lobby):
        protocol(std::move(lobby.get_protocol())),
        sdl(SDLManager()),
        input_handler(sdl, this->protocol),
        receiver(this->protocol),
        my_player(MyPlayer(lobby.get_username())) {
    if (!this->validate_qt_results(lobby)) {
        throw std::runtime_error(
                "Error creating SDL interface");  // quizas ponerlo en los get de lobby.
    }
}

void GameUI::run() {

    GameMap map = this->receiver.receive_initial_map();

    input_handler.start_sender();
    this->receiver.start();

    sdl.texto_prueba();

    Snapshot last_snapshot = this->receiver.receive_initial_snapshot();

    for (const PlayerDTO& p: last_snapshot.players) {
        my_player.update_my_position(p);
    }

    int it = 0;
    Clock clock;
    bool loop_game = true;
    while (loop_game) {
        loop_game = input_handler.handle_events();

        Snapshot snapshot_tmp;
        while (this->receiver.try_pop_snapshot_from_queue(snapshot_tmp)) {
            last_snapshot = std::move(snapshot_tmp);
        }

        for (const PlayerDTO& p: last_snapshot.players) {
            my_player.update_my_position(p);
        }

        sdl.clear_display();

        sdl.render_in_z_order(map, last_snapshot, my_player.get_username());

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
    input_handler.stop_sender();
    receiver.stop();
    input_handler.close_sender_queue();
    receiver.close_queue();
    protocol.close();
    input_handler.join_sender();
    receiver.join();
}
