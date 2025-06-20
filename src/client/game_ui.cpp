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
                   PlayerInfo{},
                   std::nullopt},
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
        local_info.server_has_been_closed = true;
    }

    this->handle_game_ended();
    this->close_client();
}

void GameUI::reset_player_events() {
    for (auto& [username, p]: local_info.players) {
        p.movement = false;
        p.shoot = false;
        p.impact_position_x = 0;
        p.impact_position_y = 0;
    }
    local_info.player.movement = false;
    local_info.player.shoot = false;
    local_info.player.impact_position_x = 0;
    local_info.player.impact_position_y = 0;
}

void GameUI::detect_player_events(const Snapshot& snapshot) {
    auto check_and_flag = [](PlayerInfo& info, const PlayerDTO& dto) {
        if (info.x != dto.position.x || info.y != dto.position.y) {
            info.movement = true;
        }
        if (dto.shot.has_value()) {
            info.shoot = true;
            info.impact_position_x = dto.shot->impact_position.x;
            info.impact_position_y = dto.shot->impact_position.y;
        }
    };

    for (auto& [username, player]: local_info.players) {
        for (const auto& dto: snapshot.ct) {
            if (dto.username == local_info.username) {
                check_and_flag(local_info.player, dto);
            } else if (dto.username == username) {
                check_and_flag(player, dto);
            }
        }
        for (const auto& dto: snapshot.tt) {
            if (dto.username == local_info.username) {
                check_and_flag(local_info.player, dto);
            } else if (dto.username == username) {
                check_and_flag(player, dto);
            }
        }
    }
}


void GameUI::update_local_info_from_snapshot(const Snapshot& snapshot) {
    update_game_status(snapshot);
    local_info.time_left = snapshot.time_left;
    local_info.bomb_status = snapshot.bomb_status;
    if (snapshot.bomb_position.has_value()) {
        local_info.bomb_planted_x = snapshot.bomb_position->x;
        local_info.bomb_planted_y = snapshot.bomb_position->y;
        std::cout << local_info.bomb_planted_x << ", " << local_info.bomb_planted_y << std::endl;
    }
    local_info.total_rounds = snapshot.total_rounds;
    local_info.current_round = snapshot.current_round_number;
    local_info.total_players = snapshot.total_players;
    local_info.phase = snapshot.phase;
    local_info.current_round_winner = snapshot.current_round_winner;
    local_info.ct_wins = snapshot.ct_wins;
    local_info.tt_wins = snapshot.tt_wins;

    for (const PlayerDTO& p: snapshot.ct) {
        update_player(p, true);
    }
    for (const PlayerDTO& p: snapshot.tt) {
        update_player(p, false);
    }
}

void GameUI::update_player(const PlayerDTO& player, const bool& is_ct) {
    if (player.username == local_info.username) {
        local_info.player.is_ct = is_ct;
        local_info.player.x = player.position.x;
        local_info.player.y = player.position.y;
        local_info.player.orientation = player.orientation;
        local_info.player.life = player.life;
        local_info.player.money = player.loadout.money;
        local_info.player.primary_gun = player.loadout.primary_gun;
        local_info.player.secondary_gun = player.loadout.secondary_gun;
        local_info.player.equipped = player.loadout.equipped;
        local_info.player.in_site = player.on_site;
        local_info.player.has_bomb = player.loadout.has_bomb;
        local_info.player.kills = player.kills;
        local_info.player.bonifications = player.bonifications;
        local_info.player.deaths = player.deaths;

        if (local_info.player.equipped == PRIMARY)
            local_info.player.equipped_gun_ammo = player.loadout.primary_ammo;
        else if (local_info.player.equipped == SECONDARY)
            local_info.player.equipped_gun_ammo = player.loadout.secondary_ammo;
        else
            local_info.player.equipped_gun_ammo = 0;
    } else {
        auto it = local_info.players.find(player.username);
        if (it != local_info.players.end()) {
            it->second.is_ct = is_ct;
            it->second.x = player.position.x;
            it->second.y = player.position.y;
            it->second.orientation = player.orientation;
            it->second.life = player.life;
            it->second.money = player.loadout.money;
            it->second.primary_gun = player.loadout.primary_gun;
            it->second.secondary_gun = player.loadout.secondary_gun;
            it->second.equipped = player.loadout.equipped;
            it->second.has_bomb = player.loadout.has_bomb;
            it->second.in_site = player.on_site;
            it->second.kills = player.kills;
            it->second.bonifications = player.bonifications;
            it->second.deaths = player.deaths;
            if (it->second.equipped == PRIMARY)
                it->second.equipped_gun_ammo = player.loadout.primary_ammo;
            else if (it->second.equipped == SECONDARY)
                it->second.equipped_gun_ammo = player.loadout.secondary_ammo;
            else
                it->second.equipped_gun_ammo = 0;
        } else {
            PlayerInfo updated;
            updated.is_ct = is_ct;
            updated.x = player.position.x;
            updated.y = player.position.y;
            updated.orientation = player.orientation;
            updated.life = player.life;
            updated.money = player.loadout.money;
            updated.primary_gun = player.loadout.primary_gun;
            updated.secondary_gun = player.loadout.secondary_gun;
            updated.equipped = player.loadout.equipped;
            updated.has_bomb = player.loadout.has_bomb;
            updated.in_site = player.on_site;
            updated.kills = player.kills;
            updated.bonifications = player.bonifications;
            updated.deaths = player.deaths;

            if (updated.equipped == PRIMARY)
                updated.equipped_gun_ammo = player.loadout.primary_ammo;
            else if (updated.equipped == SECONDARY)
                updated.equipped_gun_ammo = player.loadout.secondary_ammo;
            else
                updated.equipped_gun_ammo = 0;
            local_info.players[player.username] = std::move(updated);
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
            usernames.push_back(local_info.username);
            for (const auto& [username, player]: local_info.players) usernames.push_back(username);

            this->sdl.set_sound_info(usernames);
            return false;
        }
    }
    return true;
}

void GameUI::show_waiting(const int& it) {
    sdl.clear_display();
    sdl.render_waiting_screen(
            local_info.players.size() +
                    1,  // 1 porque si veo esta pantalla quiere decir estoy conectado
            local_info.total_players, local_info.gamename, it,
            Settings::getInstance().get_fps_client(), false);
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

void GameUI::handle_between_rounds_events() {
    this->keep_running = input_handler.handle_between_rounds_events();
}

bool GameUI::update_between_rounds() {
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
        Snapshot snapshot = std::get<Snapshot>(game_dto);
        detect_player_events(snapshot);
        last_snapshot = std::move(snapshot);
        got_snapshot = true;

        if (local_info.phase != ROUND_ENDED) {
            return false;
        }
    }
    if (got_snapshot)
        update_local_info_from_snapshot(last_snapshot);
    return true;
}

void GameUI::show_between_rounds(const int& it) {
    sdl.clear_display();
    sdl.render_in_z_order(local_info, it);
    sdl.render_crosshair(local_info);
    sdl.show_screen();
}

void GameUI::play_start_round_sound() { sdl.make_round_start_sound(local_info.player.is_ct); }

void GameUI::play_team_winner_sound() {
    sdl.make_team_winner_sound(local_info.current_round_winner);
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

void GameUI::handle_game_ended() {
    Clock clock;
    size_t last_it = 0;
    size_t it = 0;
    float time = 0.0f;
    int fps_client = Settings::getInstance().get_fps_client();
    float max_time = Settings::getInstance().get_stats_time();

    while (time < max_time) {
        if (!input_handler.handle_ended_events()) {
            break;
        }
        size_t delta_it = it - last_it;
        float delta_seconds = static_cast<float>(delta_it) / fps_client;
        time += delta_seconds;

        if (local_info.phase != WAITING_PLAYERS) {
            sdl.clear_display();
            sdl.render_in_z_order(local_info, it);
            sdl.render_crosshair(local_info);
            sdl.show_screen();
        } else {
            sdl.clear_display();
            sdl.render_waiting_screen(
                    local_info.players.size() +
                            1,  // 1 porque si veo esta pantalla quiere decir estoy conectado
                    local_info.total_players, local_info.gamename, it, fps_client, true);
            sdl.show_screen();
        }

        last_it = it;
        it = clock.sleep_and_calc_next_it(fps_client, it);
    }
}

void GameUI::change_phase(std::unique_ptr<GameUIPhase> new_phase) {
    this->phase = std::move(new_phase);
}

// just planted just defuse en local info, y las variables de sonido las pondria en Sounds, y en sdl
// segun lo que leo de local info, hago el sonido.
void GameUI::update_game_status(const Snapshot& snapshot) {
    if (local_info.bomb_status == BombStatus::NOT_PLANTED &&
        snapshot.bomb_status == BombStatus::PLANTED && !make_sound_planted) {
        sdl.make_clock_sound(false);
        make_sound_clock = false;
        sdl.make_bomb_sound(snapshot.bomb_status);
        just_planted = true;
        make_sound_planted = true;
    } else if (local_info.bomb_status == BombStatus::PLANTED &&
               snapshot.bomb_status == BombStatus::DEFUSED && !make_sound_defused) {
        sdl.make_clock_sound(false);
        sdl.make_bomb_sound(local_info.bomb_status);
        just_defuse = true;
        make_sound_defused = true;
    }
    if (snapshot.phase == ROUND_ENDED && make_sound_clock) {
        make_sound_clock = false;
        sdl.make_clock_sound(false);
    }
    if (local_info.current_round == snapshot.current_round_number - 1) {
        sdl.make_clock_sound(false);
        just_planted = false;
        just_defuse = false;
        make_sound_defused = false;
        make_sound_planted = false;
        make_sound_clock = false;
    }
    if (snapshot.time_left <= 10 && !make_sound_clock && local_info.phase == ATTACK) {
        sdl.make_clock_sound(true);
        make_sound_clock = true;
    }
}

void GameUI::close_client() {
    this->protocol.close();
    this->receiver.join();
    this->input_handler.close_sender_queue();
    this->input_handler.join_sender();
}

GameUI::~GameUI() {}
