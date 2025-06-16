#ifndef SETTINGS_H
#define SETTINGS_H

// server
#define FPS_SERVER 60
#define ROUNDS 10

#define BUY_PHASE_DURATION 10
#define ATTACK_PHASE_DURATION 60
#define WAITING_PLAYERS_PHASE_DURATION 300
#define BETWEEN_ROUNDS_PHASE_DURATION 5

#define TERRORISTS 1
#define COUNTER_TERRORISTS 1

#define GLOCK_INITIAL_AMMO 30
#define GLOCK_ROF 400
#define AWP_INITIAL_AMMO 10
#define M3_INITIAL_AMMO 20
#define AK47_INITIAL_AMMO 45
#define KNIFE_ROF 60

#define INITIAL_MONEY 20000

#define AK47_PRICE 2500
#define M3_PRICE 1700
#define AWP_PRICE 4750

#define CLIP_PRICE 50

#define SIZE_GLOCK_CLIP 50
#define SIZE_AK47_CLIP 20
#define SIZE_M3_CLIP 8
#define SIZE_AWP_CLIP 4

#define BLOCK_THICKNESS 400

#define PLAYER_SPEED 10       // debe ser un numero par
#define PLAYER_THICKNESS 320  // debe ser menor a BLOCK_THICKNESS (para spawns)
#define PLAYER_INITIAL_LIFE 100

#define PATH_FOLDER_MAPS "../maps/"
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
#define GRAPHIC_SCALE 10  // despues habria que sacarlo
#define PLAYER_SPRITE_GAP 90

struct ServerSettings {
    int fps;
    int rounds;

    int buy_phase_duration;
    int attack_phase_duration;
    int waiting_players_phase_duration;
    int between_rounds_phase_duration;

    int player_speed;
    int terrorists;
    int counter_terrorists;

    int initial_money;

    int player_width;
    int player_height;
    int player_initial_life;
};

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

struct Settings {};

#endif
