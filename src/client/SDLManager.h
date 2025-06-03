#ifndef SDLMANAGER_H
#define SDLMANAGER_H

#include <string>
#include <utility>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/game_map.h"
#include "../common/game_snapshot.h"

#include "camera.h"

#define WINDOW_INITIAL_WIDTH 640
#define WINDOW_INITIAL_HEIGHT 400
#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 400

#define BOX_X_POS_SPRITE 416
#define BOX_Y_POS_SPRITE 64
#define SIZE_BOX 32

#define PLAYER_X_POS_SPRITE 0
#define PLAYER_Y_POS_SPRITE 32
#define SIZE_PLAYER 32


class SDLManager {
private:
    SDL2pp::SDL sdl;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;

    SDL2pp::Surface playerSheet;
    SDL2pp::Texture player;

    SDL2pp::Surface boxSheet;
    SDL2pp::Texture box;

    SDL2pp::Surface waitingBackgroundSheet;
    SDL2pp::Texture waitingBackground;

    SDL2pp::Surface hudNumbersSheet;
    SDL2pp::Texture hudNumbers;

    SDL2pp::Surface hudSymbolsSheet;
    SDL2pp::Texture hudSymbols;

    Camera camera;

    void update_camera(int player_x, int player_y);
    std::pair<float, float> get_scales() const;
    float get_uniform_scale() const;
    SDL2pp::Point get_render_offset() const;
    void render_player(const PlayerDTO& p, float scale);
    void render_hud_time(int time_left, float scale);
    void render_hud_life(uint16_t life, float scale);

public:
    SDLManager();

    void render_waiting_screen(int players_connected, int players_required,
                               const std::string& gamename, int iteration, int FPS);
    std::pair<int, int> get_window_size() const;
    void clear_display();
    void render_in_z_order(const GameMap& map, const Snapshot& snapshot,
                           const std::string& my_username);
    void show_screen();
};

#endif
