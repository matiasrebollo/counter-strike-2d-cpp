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

    int block_thickness;
    int player_thickness;  // debe ser menor a BLOCK_THICKNESS (para spawns)
    int player_initial_life;

    int initial_money;
    int clip_price;
    float damage_variation_factor;

    std::unordered_map<GunType, GunSettings> guns_settings;
    KnifeSettings knife_settings;
};

#define PATH_FOLDER_MAPS "../maps/"
#define PATH_SETTINGS "../settings.yaml"
#define PATH_CS_FONT "../../../assets/cs_regular.ttf"


// client
#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 400
#define FPS_CLIENT 30

#define WINDOW_INITIAL_WIDTH 640
#define WINDOW_INITIAL_HEIGHT 400
#define HUD_IDEAL_WIDTH 640
#define HUD_IDEAL_HEIGHT 400
#define FONT_IDEAL_WIDTH 480
#define FONT_IDEAL_HEIGHT 300
#define GRAPHIC_SCALE 10


struct ClientSettings {
    int camera_width;
    int camera_height;

    int fps;

    int window_initial_width;
    int window_initial_height;
    int hud_ideal_width;
    int hud_ideal_height;
    int font_ideal_width;
    int font_ideal_height;
    int size_player;
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
    int get_block_thickness();
    int get_player_thickness();
    int get_player_initial_life();
    int get_initial_money();
    int get_clip_price();
    GunSettings get_gun(const GunType& gun);
    float get_damage_variation_factor();

    int get_gun_price(const GunType& gun);
    int get_clip_size(const GunType& gun);
    int get_gun_rof(const GunType& gun);
    int get_gun_dmg(const GunType& gun);
    int get_gun_falloff(const GunType& gun);
    int get_gun_precision(const GunType& gun);
    int get_gun_initial_ammo(const GunType& gun);
    int get_gun_kill_bonus(const GunType& gun);

    int get_knife_dmg();
    int get_knife_ar();
    int get_knife_distance();
    int get_knife_kill_bonus();

    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;
    Settings(Settings&&) = delete;
    Settings& operator=(Settings&&) = delete;

private:
    Settings();
    ~Settings() = default;

    ServerSettings serverSettings;
    // ClientSettings clientSettings;
};


#endif
