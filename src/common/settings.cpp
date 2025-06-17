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

int Settings::get_initial_money() { return serverSettings.initial_money; }

int Settings::get_clip_price() { return serverSettings.clip_price; }

int Settings::get_gun_price(const GunType& gun) { return serverSettings.guns_settings[gun].price; }

int Settings::get_clip_size(const GunType& gun) {
    return serverSettings.guns_settings[gun].clip_size;
}

int Settings::get_gun_rof(const GunType& gun) { return serverSettings.guns_settings[gun].rof; }
int Settings::get_gun_dmg(const GunType& gun) { return serverSettings.guns_settings[gun].dmg; }
int Settings::get_gun_falloff(const GunType& gun) {
    return serverSettings.guns_settings[gun].falloff;
}
int Settings::get_gun_precision(const GunType& gun) {
    return serverSettings.guns_settings[gun].precision;
}
int Settings::get_gun_initial_ammo(const GunType& gun) {
    return serverSettings.guns_settings[gun].initial_ammo;
}
int Settings::get_gun_kill_bonus(const GunType& gun) {
    return serverSettings.guns_settings[gun].kill_bonus;
}

GunSettings Settings::get_gun(const GunType& gun) { return serverSettings.guns_settings[gun]; }
float Settings::get_damage_variation_factor() { return serverSettings.damage_variation_factor; }

int Settings::get_knife_ar() { return serverSettings.knife_settings.knife_ar; }
int Settings::get_knife_distance() { return serverSettings.knife_settings.knife_distance; }
int Settings::get_knife_dmg() { return serverSettings.knife_settings.knife_dmg; }
int Settings::get_knife_kill_bonus() { return serverSettings.knife_settings.knife_kill_bonus; }
