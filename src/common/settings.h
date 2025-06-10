#ifndef SETTINGS_H
#define SETTINGS_H

// server
#define FPS_SERVER 60
#define ROUNDS 10

#define BUY_PHASE_DURATION 15
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
#define CLIP_PRICE 50

#define PLAYER_SPEED 120
#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32
#define PLAYER_INITIAL_LIFE 100

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
#define SIZE_PLAYER 32

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
