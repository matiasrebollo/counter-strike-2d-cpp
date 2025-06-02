#include "server/cs2d_game.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

CS2DGame::CS2DGame(const std::string& id): id(id) {
    phase = std::make_unique<WaitingPlayersPhase>(*this);
}

bool CS2DGame::can_add_player() const {
    return players_senders.size() < COUNTER_TERRORISTS + TERRORISTS;
}

bool CS2DGame::should_start() const {
    return players_senders.size() >= COUNTER_TERRORISTS + TERRORISTS;
}

void CS2DGame::add_player(const std::string& username, std::shared_ptr<ClientSender> sender) {
    if (players_senders.contains(username)) {
        throw std::runtime_error("Username '" + username + "' is already in the game.");
    }

    game_world.add_player(username);
    players_senders[username] = sender;
}

void CS2DGame::push(std::unique_ptr<Command> command) { command_queue.push(std::move(command)); }

void CS2DGame::broadcast_game_dto(const GameDTO& game_dto) const {
    for (const auto& [_, sender]: players_senders) {
        sender->send_game_dto(game_dto);
    }
}

void CS2DGame::broadcast_map() const {
    const GameMap map = game_world.get_map();
    broadcast_game_dto(map);
}

void CS2DGame::broadcast_snapshot() const {
    const Snapshot snapshot = game_world.get_snapshot();
    broadcast_game_dto(snapshot);
}

void CS2DGame::update(const size_t& it, size_t& prev_it) {
    for (size_t i = 0; i < (it + 1) - prev_it; ++i) {
        game_world.update();
    }
    prev_it = it;
}

void CS2DGame::execute_in_attack_phase(std::unique_ptr<Command> cmd) {
    cmd->execute_in_attack_phase(this->game_world);
}

void CS2DGame::execute_in_buy_phase(std::unique_ptr<Command> cmd) {
    cmd->execute_in_buy_phase(this->game_world);
}

void CS2DGame::end_attack_phase() {
    // limpiar items del mapa (dejar algunos)
    // reiniciar posiciones de cada jugador al spawn
    // ver que equipo ganó y contabilizar!
}

void CS2DGame::change_phase(std::unique_ptr<GamePhase> new_phase) {
    this->phase = std::move(new_phase);
    if (this->phase->type() == BUY)
        this->round++;
    // algo mas??
}

void CS2DGame::swap_teams() {}

void CS2DGame::end_game() {
    // finalizar partida (llamar a stop())
    // determinar equipo ganador y enviar stadisticas finales
    this->command_queue.close();
    this->stop();
    // los mapas deberian liberarse solos porque son RAII al igual que los shared_ptr
}

void CS2DGame::run() {
    while (should_keep_running()) {
        if (this->round > ROUNDS) {
            end_game();
            continue;
        }
        if (this->round == ROUNDS / 2)
            swap_teams();
        phase->run();
    }
}

CS2DGame::~CS2DGame() {}
