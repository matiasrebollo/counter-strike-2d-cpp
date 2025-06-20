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

    std::unordered_map<GunType, GunSettings> guns_settings;
    KnifeSettings knife_settings;
};

#define BLOCK_THICKNESS 400
#define PLAYER_THICKNESS 320

#define MAX_DISTANCE_SHOT 10000

#define TEAM_KILL_PENALTY 3000
#define AWP_KILL_BONUS 100
#define AK47_KILL_BONUS 300
#define M3_KILL_BONUS 300
#define GLOCK_KILL_BONUS 500
#define KNIFE_KILL_BONUS 1500
#define WON_ROUND_BONUS 3250
#define LOST_ROUND_BONUS 2000

#define BOMB_THICKNESS 100

#define BOMB_EXPLOSION_DAMAGE 200
#define BOMB_EXPLOSION_RADIUS 1000
#define DETONATION_TIME 30
#define PLANTATION_TIME 4
#define DEFUSE_TIME 4

#define PATH_FOLDER_MAPS "../maps/"
#define PATH_SETTINGS "../settings.yaml"
#define PATH_CS_FONT "../../../assets/cs_regular.ttf"

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
    int get_clip_price();
    GunSettings get_gun(const GunType& gun);
    float get_damage_variation_factor();

    int get_gun_price(const GunType& gun);
    int get_clip_size(const GunType& gun);

    int get_knife_dmg();
    int get_knife_ar();
    int get_knife_distance();
    int get_knife_kill_bonus();

    int get_fps_client();
    int get_window_initial_width();
    int get_window_initial_height();
    bool get_fullscreen();

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
