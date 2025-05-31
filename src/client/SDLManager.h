#ifndef SDLMANAGER_H
#define SDLMANAGER_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/game_map.h"
#include "../common/game_snapshot.h"

#include "camera.h"

#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH 240
#define CAMERA_HEIGHT 320
#define CAMERA_WIDTH 240

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

    Camera camera;

    void update_camera(int player_x, int player_y);

public:
    SDLManager();

    double calculate_angle_to_mouse(int mouse_x, int mouse_y) const;
    void clear_display();
    void show_screen();
    void render_in_z_order(const GameMap& map, const Snapshot& snapshot,
                           const std::string& my_username);
    void texto_prueba();
};

#endif
