#ifndef SETTINGS_H
#define SETTINGS_H

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
