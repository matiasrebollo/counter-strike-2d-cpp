#include "settings.h"

#include "yaml_parser.h"

Settings::Settings(): serverSettings(YamlParser().load_server_settings(PATH_SETTINGS)) {}

Settings& Settings::getInstance() {
    static Settings instance;
    return instance;
}

int Settings::get_fps_server() { return serverSettings.fps; }

size_t Settings::get_rounds_server() { return serverSettings.rounds; }

int Settings::get_buy_phase_duration() { return serverSettings.buy_phase_duration; }

int Settings::get_attack_phase_duration() { return serverSettings.attack_phase_duration; }

int Settings::get_waiting_phase_duration() { return serverSettings.waiting_players_phase_duration; }

int Settings::get_between_rounds_phase_duration() {
    return serverSettings.between_rounds_phase_duration;
}

int Settings::get_player_speed() { return serverSettings.player_speed; }

size_t Settings::get_terrorists_number() { return serverSettings.terrorists; }

size_t Settings::get_counter_terrorists_number() { return serverSettings.counter_terrorists; }

int Settings::get_block_thickness() { return serverSettings.block_thickness; }

int Settings::get_player_thickness() { return serverSettings.player_thickness; }

int Settings::get_player_initial_life() { return serverSettings.player_initial_life; }
