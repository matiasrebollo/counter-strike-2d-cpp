#include "game_ui.h"

#include <optional>
#include <variant>

#include <unistd.h>

#include "common/game_dto.h"

GameUI::GameUI(Lobby& lobby):
        protocol(std::move(lobby.get_protocol())),
        sdl(SDLManager()),
        input_handler(sdl, this->protocol),
        receiver(this->protocol),
        // podria usar move?
        local_info{lobby.get_username(),
                   lobby.get_gamecode(),
                   lobby.get_ct_skin(),
                   lobby.get_tt_skin(),
                   {},
                   {},
                   PlayerInfo{}},
        keep_running(true) {
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

void GameUI::reset_player_events() {
    for (auto& p: local_info.ct_players) {
        p.movement = false;
        p.shoot = false;
        p.shot_distance = 0;
    }
    for (auto& p: local_info.tt_players) {
        p.movement = false;
        p.shoot = false;
        p.shot_distance = 0;
    }
    local_info.player.movement = false;
    local_info.player.shoot = false;
    local_info.player.shot_distance = 0;
}

void GameUI::detect_player_events(const Snapshot& snapshot) {
    auto check_and_flag = [](PlayerInfo& info, const PlayerDTO& dto) {
        if (info.x != dto.position.x || info.y != dto.position.y) {
            info.movement = true;
        }
        if (dto.shot.has_value()) {
            info.shoot = true;
            info.shot_distance = dto.shot->distance;
            std::cout << "distancia protocolo: " << info.shot_distance << std::endl;
        }
    };

    for (const auto& dto: snapshot.ct) {
        if (dto.username == local_info.username) {
            check_and_flag(local_info.player, dto);
            continue;
        }

        for (auto& info: local_info.ct_players) {
            if (dto.username == info.username) {
                check_and_flag(info, dto);
                break;
            }
        }
    }

    for (const auto& dto: snapshot.tt) {
        if (dto.username == local_info.username) {
            check_and_flag(local_info.player, dto);
            continue;
        }

        for (auto& info: local_info.tt_players) {
            if (dto.username == info.username) {
                check_and_flag(info, dto);
                break;
            }
        }
    }
}


void GameUI::update_local_info_from_snapshot(const Snapshot& snapshot) {
    local_info.time_left = snapshot.time_left;
    local_info.total_players = snapshot.total_players;
    local_info.phase = snapshot.phase;

    // Actualizo jugadores CT
    for (const PlayerDTO& p: snapshot.ct) {
        bool found = false;
        if (p.username == local_info.username) {
            local_info.player.username = p.username;
            local_info.player.x = p.position.x;
            local_info.player.y = p.position.y;
            local_info.player.orientation = p.orientation;
            local_info.player.life = p.life;
            local_info.player.money = p.loadout.money;
            local_info.player.primary_gun = p.loadout.primary_gun;
            local_info.player.secondary_gun = p.loadout.secondary_gun;
            local_info.player.equipped = p.loadout.equipped;

            if (local_info.player.equipped == PRIMARY)
                local_info.player.equipped_gun_ammo = p.loadout.primary_ammo;
            else if (local_info.player.equipped == SECONDARY)
                local_info.player.equipped_gun_ammo = p.loadout.secondary_ammo;
            else
                local_info.player.equipped_gun_ammo = 0;
            found = true;
        } else {
            // Busco el jugador en ct_players y actualizo
            for (auto& info: local_info.ct_players) {
                if (info.username == p.username) {
                    info.username = p.username;
                    info.x = p.position.x;
                    info.y = p.position.y;
                    info.orientation = p.orientation;
                    info.life = p.life;
                    info.money = p.loadout.money;
                    info.primary_gun = p.loadout.primary_gun;
                    info.secondary_gun = p.loadout.secondary_gun;
                    info.equipped = p.loadout.equipped;

                    if (info.equipped == PRIMARY)
                        info.equipped_gun_ammo = p.loadout.primary_ammo;
                    else if (info.equipped == SECONDARY)
                        info.equipped_gun_ammo = p.loadout.secondary_ammo;
                    else
                        info.equipped_gun_ammo = 0;
                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            // No lo encontré ni en local ni en ct_players → agrego nuevo
            PlayerInfo info;
            info.username = p.username;
            info.is_ct = true;
            info.x = p.position.x;
            info.y = p.position.y;
            info.orientation = p.orientation;
            info.life = p.life;
            info.money = p.loadout.money;
            info.primary_gun = p.loadout.primary_gun;
            info.secondary_gun = p.loadout.secondary_gun;
            info.equipped = p.loadout.equipped;

            if (p.loadout.equipped == PRIMARY)
                info.equipped_gun_ammo = p.loadout.primary_ammo;
            else if (p.loadout.equipped == SECONDARY)
                info.equipped_gun_ammo = p.loadout.secondary_ammo;
            else
                info.equipped_gun_ammo = 0;

            local_info.ct_players.push_back(std::move(info));
        }
    }

    // Actualizo jugadores TT (igual que CT)
    for (const PlayerDTO& p: snapshot.tt) {
        bool found = false;
        if (p.username == local_info.username) {
            local_info.player.username = p.username;
            local_info.player.x = p.position.x;
            local_info.player.y = p.position.y;
            local_info.player.orientation = p.orientation;
            local_info.player.life = p.life;
            local_info.player.money = p.loadout.money;
            local_info.player.primary_gun = p.loadout.primary_gun;
            local_info.player.secondary_gun = p.loadout.secondary_gun;
            local_info.player.equipped = p.loadout.equipped;

            if (local_info.player.equipped == PRIMARY)
                local_info.player.equipped_gun_ammo = p.loadout.primary_ammo;
            else if (local_info.player.equipped == SECONDARY)
                local_info.player.equipped_gun_ammo = p.loadout.secondary_ammo;
            else
                local_info.player.equipped_gun_ammo = 0;
            found = true;
        } else {
            for (auto& info: local_info.tt_players) {
                if (info.username == p.username) {
                    info.username = p.username;
                    info.x = p.position.x;
                    info.y = p.position.y;
                    info.orientation = p.orientation;
                    info.life = p.life;
                    info.money = p.loadout.money;
                    info.primary_gun = p.loadout.primary_gun;
                    info.secondary_gun = p.loadout.secondary_gun;
                    info.equipped = p.loadout.equipped;

                    if (info.equipped == PRIMARY)
                        info.equipped_gun_ammo = p.loadout.primary_ammo;
                    else if (info.equipped == SECONDARY)
                        info.equipped_gun_ammo = p.loadout.secondary_ammo;
                    else
                        info.equipped_gun_ammo = 0;
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            // No lo encontré ni en local ni en ct_players → agrego nuevo
            PlayerInfo info;
            info.username = p.username;
            info.is_ct = false;
            info.x = p.position.x;
            info.y = p.position.y;
            info.orientation = p.orientation;
            info.life = p.life;
            info.money = p.loadout.money;
            info.primary_gun = p.loadout.primary_gun;
            info.secondary_gun = p.loadout.secondary_gun;
            info.equipped = p.loadout.equipped;

            if (p.loadout.equipped == PRIMARY)
                info.equipped_gun_ammo = p.loadout.primary_ammo;
            else if (p.loadout.equipped == SECONDARY)
                info.equipped_gun_ammo = p.loadout.secondary_ammo;
            else
                info.equipped_gun_ammo = 0;

            local_info.tt_players.push_back(std::move(info));
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
                        update_local_info_from_snapshot(std::move(game_dto));
                    } else if constexpr (std::is_same_v<T, GameInitialInfoDTO>) {
                        this->sdl.set_map(std::move(game_dto.game_map));
                        this->sdl.set_shop(std::move(game_dto.shop_info));
                        pop = false;
                    } else if constexpr (std::is_same_v<T, GameEnded>) {
                        // guardar estadisticas
                        // estado ended?
                        this->keep_running = false;
                        pop = false;
                    }
                },
                game_dto);
        if (!pop)
            continue;
        if (local_info.phase != WAITING_PLAYERS) {
            std::vector<std::string> usernames;
            usernames.push_back(local_info.player.username);
            for (const auto& player: local_info.ct_players) usernames.push_back(player.username);
            for (const auto& player: local_info.tt_players) usernames.push_back(player.username);

            this->sdl.set_sound_info(usernames);
            return false;
        }
    }
    return true;
}

void GameUI::show_waiting(const int& it) {
    sdl.clear_display();
    sdl.render_waiting_screen(
            local_info.ct_players.size() + local_info.tt_players.size() +
                    1,  // 1 porque si veo esta pantalla quiere decir estoy conectado
            local_info.total_players, local_info.gamename, it, FPS_CLIENT);
    sdl.show_screen();
}

void GameUI::handle_buy_events() {
    this->keep_running =
            input_handler.handle_buy_events(local_info.player.money, local_info.player.primary_gun);
}
bool GameUI::update_buy() {
    GameDTO game_dto;
    bool pop = true;
    while (pop) {
        if (!this->receiver.try_pop_game_dto(game_dto)) {
            pop = false;
            continue;
        }
        update_local_info_from_snapshot(std::get<Snapshot>(game_dto));

        if (local_info.phase != BUY) {
            return false;
        }
    }
    return true;
}

void GameUI::show_buy(const int& it) {
    sdl.clear_display();
    sdl.render_in_z_order(local_info, it);
    sdl.render_shop(local_info.player.money, local_info.player.primary_gun,
                    local_info.player.secondary_gun);
    sdl.render_crosshair(local_info);
    sdl.show_screen();
}

void GameUI::handle_attack_events() { this->keep_running = input_handler.handle_attack_events(); }
bool GameUI::update_attack() {
    GameDTO game_dto;
    Snapshot last_snapshot;
    bool got_snapshot = false;
    bool pop = true;

    reset_player_events();
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

        // Identificar en snapshot_tmp cambios/eventos para activar animaciones
        Snapshot snapshot = std::get<Snapshot>(game_dto);
        detect_player_events(snapshot);
        last_snapshot = std::move(snapshot);
        got_snapshot = true;

        if (local_info.phase != ATTACK) {
            return false;
        }
    }
    if (got_snapshot)
        update_local_info_from_snapshot(last_snapshot);
    return true;
}

void GameUI::show_attack(const int& it) {
    sdl.clear_display();
    sdl.render_in_z_order(local_info, it);
    sdl.render_crosshair(local_info);
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

void GameUI::close_client() {
    this->protocol.close();
    this->receiver.join();
    this->input_handler.close_sender_queue();
    this->input_handler.join_sender();
}

GameUI::~GameUI() {
    if (this->keep_running) {
        this->close_client();
    }
}
