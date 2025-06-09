#ifndef SDLMANAGER_H
#define SDLMANAGER_H

#include <optional>
#include <string>
#include <utility>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/block_texture_parser.h"
#include "../common/game_map.h"
#include "../common/game_snapshot.h"

#include "camera.h"
#include "local_info.h"
#include "shop.h"

#define WINDOW_INITIAL_WIDTH 640
#define WINDOW_INITIAL_HEIGHT 400
#define HUD_IDEAL_WIDTH 640
#define HUD_IDEAL_HEIGHT 400
#define FONT_IDEAL_WIDTH 480
#define FONT_IDEAL_HEIGHT 300
#define SIZE_PLAYER 32

class SDLManager {
private:
    BlockTextureParser texture_parser;
    SDL2pp::SDL sdl;
    SDL2pp::SDLTTF ttf;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;
    TextureManager texture_manager;
    Camera camera;
    Shop shop;

    void update_camera(int player_x, int player_y);
    std::pair<Position, GunSprites> get_gun_info(const LoadoutDTO& loadout);
    void render_player(const PlayerDTO& p, const BlockTextureInfo& sprite_info);
    void render_player_weapon(const PlayerDTO& p);
    void render_hud_time(int time_left);
    void render_hud_life(uint16_t life);
    Crosshairs get_crosshair_color(int mouse_x, int mouse_y, const Snapshot& snapshot,
                                   const LocalInfo& local_info);

public:
    SDLManager();

    void render_waiting_screen(int players_connected, int players_required,
                               const std::string& gamename, int iteration, int FPS);
    SDL_Point get_logical_size() const;
    void clear_display();
    void render_in_z_order(const GameMap& map, const Snapshot& snapshot,
                           const LocalInfo& local_info);
    std::optional<ShopButtonType> get_clicked_button(int x, int y);
    void render_shop();
    void render_crosshair(const Snapshot& snapshot, const LocalInfo& local_info);
    void show_screen();
};

#endif
