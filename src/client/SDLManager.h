#ifndef SDLMANAGER_H
#define SDLMANAGER_H

#include <string>
#include <utility>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/block_texture_parser.h"
#include "../common/game_map.h"
#include "../common/game_snapshot.h"

#include "camera.h"
#include "local_player_info.h"
#include "texture_manager.h"

#define WINDOW_INITIAL_WIDTH 640
#define WINDOW_INITIAL_HEIGHT 400
#define CAMERA_WIDTH 480
#define CAMERA_HEIGHT 300
#define HUD_IDEAL_WIDTH 640
#define HUD_IDEAL_HEIGHT 400
#define FONT_IDEAL_WIDTH 480
#define FONT_IDEAL_HEIGHT 300

#define PLAYER_X_POS_SPRITE 0
#define PLAYER_Y_POS_SPRITE 32
#define SIZE_PLAYER 32


class SDLManager {
private:
    SDL2pp::SDL sdl;
    SDL2pp::SDLTTF ttf;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;
    TextureManager texture_manager;
    BlockTextureParser texture_parser;
    Camera camera;

    void update_camera(int player_x, int player_y);
    float get_scale_for(int width, int height) const;
    void render_player(const PlayerDTO& p, const BlockTextureInfo& sprite_info);
    void render_hud_time(int time_left);
    void render_hud_life(uint16_t life);

public:
    SDLManager();

    void render_waiting_screen(int players_connected, int players_required,
                               const std::string& gamename, int iteration, int FPS);
    SDL_Point get_logical_size() const;
    void clear_display();
    void render_in_z_order(const GameMap& map, const Snapshot& snapshot,
                           const LocalPlayerInfo& local_info);
    void show_screen();
};

#endif
