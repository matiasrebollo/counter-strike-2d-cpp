#include "server/cs2d_game.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "common/clock.h"
#include "common/game_map.h"
#include "common/game_snapshot.h"
#include "common/yaml_parser.h"

CS2DGame::CS2DGame(const std::string& id):
        players_senders(),
        command_queue(),
        game_world(),
        current_round(0),
        current_round_winner(std::nullopt),
        ct_wins(0),
        tt_wins(0),
        id(id) {
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
        if (sender->is_alive()) {
            sender->send_game_dto(game_dto);
        }
    }
}

void CS2DGame::broadcast_map() const {
    const GameMap map = game_world.get_map();
    broadcast_game_dto(map);
}

void CS2DGame::broadcast_snapshot(const int time_left) const {
    const GameWorldSnapshot game_world_snapshot = game_world.get_snapshot();
    const Snapshot snapshot{
            this->phase->type(), this->current_round,    ROUNDS,
            time_left,           game_world_snapshot.ct, game_world_snapshot.tt,
            // this->current_round_winner
    };
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

bool CS2DGame::current_round_has_a_winner() const {
    return game_world.tt_are_all_dead() ||
           game_world.ct_are_all_dead();  // agregar detonacion de bomba
}

void CS2DGame::decide_winner() {
    if (!current_round_has_a_winner() or
        game_world.tt_are_all_dead()) {  // agregar desactivacion de bomba
        this->current_round_winner = CT;
        this->ct_wins++;
    } else if (game_world.ct_are_all_dead()) {  // agregar detonacion de bomba
        this->current_round_winner = TT;
        this->tt_wins++;
    }
}

void CS2DGame::begin_new_round() {
    this->current_round_winner = std::nullopt;
    this->current_round++;
    if (this->current_round == (ROUNDS / 2) + 1)
        swap_teams();
    game_world.stop_players();
    game_world.spawn_players();
    // limpiar items del mapa (dejar algunos, random)
}

void CS2DGame::change_phase(std::unique_ptr<GamePhase> new_phase) {
    this->phase = std::move(new_phase);
}

void CS2DGame::swap_teams() {}

void CS2DGame::end_game() {
    // determinar equipo ganador y enviar estadisticas finales
    this->command_queue.close();
    this->broadcast_game_dto(GameEnded{});
    this->stop();
}

void CS2DGame::run() {
    while (should_keep_running()) {
        if (this->current_round > ROUNDS) {
            end_game();
            break;
        }
        phase->run();
    }
}

CS2DGame::~CS2DGame() {}
