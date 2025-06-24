#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <unordered_map>

#include "weapon_type.h"

struct GunSettings {
    int initial_ammo;
    float precision;
    int falloff;
    int dmg;
    int rof;
    int price;
    int clip_size;
    int kill_bonus;
};

struct KnifeSettings {
    int knife_distance;
    int knife_dmg;
    int knife_ar;
    int knife_kill_bonus;
};

struct BombSettings {
    int explosion_damage;
    int explosion_radius;
    int detonation_time;
    int plantation_time;
    int defuse_time;
};

struct ServerSettings {
    int fps;
    size_t rounds;

    int buy_phase_duration;
    int attack_phase_duration;
    int waiting_players_phase_duration;
    int between_rounds_phase_duration;

    int player_speed;  // debe ser un numero par
    size_t terrorists;
    size_t counter_terrorists;

    int player_initial_life;

    int initial_money;
    int clip_price;
    float damage_variation_factor;

    int team_kill_penalty;
    int won_round_bonus;
    int lost_round_bonus;

    std::unordered_map<GunType, GunSettings> guns_settings;
    KnifeSettings knife_settings;
    BombSettings bomb_settings;
};

#define BLOCK_THICKNESS 400
#define PLAYER_THICKNESS 320
#define ITEM_THICKNESS 200
#define MAX_DISTANCE_SHOT 10000
#define BOMB_THICKNESS 100


#define PATH_FOLDER_MAPS "/etc/cs2d-remake/maps/"
#define PATH_SETTINGS "/etc/cs2d-remake/settings.yaml"
#define PATH_CS_FONT "/var/cs2d-remake/cs_regular.ttf"

// client
#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 400

#define HUD_IDEAL_WIDTH 640
#define HUD_IDEAL_HEIGHT 400
#define FONT_IDEAL_WIDTH 480
#define FONT_IDEAL_HEIGHT 300
#define GRAPHIC_SCALE 10
#define PLAYER_SPRITE_GAP 90


struct ClientSettings {
    // int camera_width;
    // int camera_height;
    int fps;

    int window_initial_width;
    int window_initial_height;

    bool fullscreen;

    int fov_angle;
    std::string sdl_window_title;
    float stats_time;
};

class Settings {
public:
    static Settings& getInstance();

    int get_fps_server();
    size_t get_rounds_server();
    int get_buy_phase_duration();
    int get_attack_phase_duration();
    int get_between_rounds_phase_duration();
    int get_waiting_phase_duration();
    int get_player_speed();
    size_t get_terrorists_number();
    size_t get_counter_terrorists_number();
    int get_player_initial_life();
    int get_initial_money();
    int get_team_kill_penalty();
    int get_won_round_bonus();
    int get_lost_round_bonus();
    int get_clip_price();
    GunSettings get_gun(const GunType& gun);
    float get_damage_variation_factor();

    int get_gun_price(const GunType& gun);
    int get_clip_size(const GunType& gun);

    int get_knife_dmg();
    int get_knife_ar();
    int get_knife_distance();
    int get_knife_kill_bonus();

    int get_bomb_damage();
    int get_explosion_radius();
    int get_detonation_time();
    int get_plantation_time();
    int get_defuse_time();

    int get_fps_client();
    int get_window_initial_width();
    int get_window_initial_height();
    bool get_fullscreen();
    const std::string& get_sdl_window_title();
    int get_fov_angle();
    float get_stats_time();

    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;
    Settings(Settings&&) = delete;
    Settings& operator=(Settings&&) = delete;

private:
    Settings();
    ~Settings() = default;

    ServerSettings serverSettings;
    ClientSettings clientSettings;
};


#endif
