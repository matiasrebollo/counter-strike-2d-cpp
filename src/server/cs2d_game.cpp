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

CS2DGame::CS2DGame(const std::string& id): phase(WAITING_PLAYERS), phase_time(0.0f), id(id) {}

bool CS2DGame::can_add_player() const { return players_senders.size() < MAX_PLAYERS; }

bool CS2DGame::should_start() const { return players_senders.size() >= MIN_PLAYERS; }

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

void CS2DGame::update(const size_t& it) {
    for (size_t i = 0; i < it - this->last_it; ++i) {
        game_world.update();
    }
    this->last_it = it;
}

void CS2DGame::end_attack_phase() {
    // limpiar items del mapa (dejar algunos)
    // reiniciar posiciones de cada jugador al spawn
    // ver que equipo ganó!
}

void CS2DGame::start_phase(const Phase new_phase) {
    if (new_phase == BUY)
        this->round += 1;
    this->phase = new_phase;
    this->last_it = 0;
    this->phase_time = 0.0f;
}

void CS2DGame::swap_teams() {
    // cambiar de equipos
    // cambiar skins a cada jugador (segun las que seleccionó)
}

void CS2DGame::end_game() {
    // finalizar partida (llamar a stop()?)
    // determinar equipo ganador y enviar estadisticas finales
}

void CS2DGame::run() {
    int FPS = 60;
    Clock clock;
    size_t it = 1;
    while (should_keep_running()) {
        if (this->round > ROUNDS)
            end_game();
        if (this->round == ROUNDS / 2)
            swap_teams();
        size_t delta_it = it - last_it;
        float delta_seconds = static_cast<float>(delta_it) / FPS;
        this->phase_time += delta_seconds;
        if (this->phase == WAITING_PLAYERS) {
            std::unique_ptr<Command> cmd;
            while (command_queue.try_pop(cmd)) {}
            this->last_it = it;
            // std::cout << "esperando jugadores... " << this->phase_time << std::endl;
            // agregar tiempo maximo de espera jugadores??
            // en ese caso, qué hacer si el juego termina forzadamente??
            if (this->should_start()) {
                broadcast_map();
                // broadcast_snapshot();
                start_phase(BUY);
                it = 1;
            }
            // it = clock.sleep_and_calc_next_it(FPS, it);
            // continue;
        } else if (this->phase == BUY) {
            std::unique_ptr<Command> cmd;
            while (command_queue.try_pop(cmd)) {
                if (cmd->type() == BuyPhase)
                    cmd->execute(this->game_world);
            }
            this->last_it = it;
            std::cout << this->phase_time << std::endl;
            if (this->phase_time >= BUY_PHASE_DURATION) {
                start_phase(ATTACK);
                it = 1;
            }
        } else if (this->phase == ATTACK) {
            std::unique_ptr<Command> cmd;
            while (command_queue.try_pop(cmd)) {
                if (cmd->type() == AttackPhase)
                    cmd->execute(this->game_world);
            }
            update(it);
            if (this->phase_time >= ATTACK_PHASE_DURATION) {
                end_attack_phase();
                start_phase(BUY);
                it = 1;
            }
        }
        broadcast_snapshot();
        // std::cout << "last it: " << it << std::endl;
        it = clock.sleep_and_calc_next_it(FPS, it);
        // std::cout << "new it: " << it << std::endl;
    }
}

CS2DGame::~CS2DGame() {}
