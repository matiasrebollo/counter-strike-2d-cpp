#include "settings.h"

#include "yaml_parser.h"

Settings::Settings():
        serverSettings(YamlParser().load_server_settings(PATH_SETTINGS)),
        clientSettings(YamlParser().load_client_settings(PATH_SETTINGS)) {}

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

int Settings::get_player_initial_life() { return serverSettings.player_initial_life; }

int Settings::get_initial_money() { return serverSettings.initial_money; }

int Settings::get_team_kill_penalty() { return serverSettings.team_kill_penalty; }

int Settings::get_won_round_bonus() { return serverSettings.won_round_bonus; }

int Settings::get_lost_round_bonus() { return serverSettings.lost_round_bonus; }

int Settings::get_clip_price() { return serverSettings.clip_price; }

int Settings::get_gun_price(const GunType& gun) { return serverSettings.guns_settings[gun].price; }

int Settings::get_clip_size(const GunType& gun) {
    return serverSettings.guns_settings[gun].clip_size;
}

GunSettings Settings::get_gun(const GunType& gun) { return serverSettings.guns_settings[gun]; }
float Settings::get_damage_variation_factor() { return serverSettings.damage_variation_factor; }

int Settings::get_knife_ar() { return serverSettings.knife_settings.knife_ar; }
int Settings::get_knife_distance() { return serverSettings.knife_settings.knife_distance; }
int Settings::get_knife_dmg() { return serverSettings.knife_settings.knife_dmg; }
int Settings::get_knife_kill_bonus() { return serverSettings.knife_settings.knife_kill_bonus; }

int Settings::get_bomb_damage() { return serverSettings.bomb_settings.explosion_damage; }
int Settings::get_explosion_radius() { return serverSettings.bomb_settings.explosion_radius; }
int Settings::get_plantation_time() { return serverSettings.bomb_settings.plantation_time; }
int Settings::get_detonation_time() { return serverSettings.bomb_settings.detonation_time; }
int Settings::get_defuse_time() { return serverSettings.bomb_settings.defuse_time; }

int Settings::get_fps_client() { return clientSettings.fps; }
int Settings::get_window_initial_height() { return clientSettings.window_initial_height; }
int Settings::get_window_initial_width() { return clientSettings.window_initial_width; }
bool Settings::get_fullscreen() { return clientSettings.fullscreen; }
const std::string& Settings::get_sdl_window_title() { return clientSettings.sdl_window_title; }
int Settings::get_fov_angle() { return clientSettings.fov_angle; }
float Settings::get_stats_time() { return clientSettings.stats_time; }
