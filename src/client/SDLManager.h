#ifndef SDLMANAGER_H
#define SDLMANAGER_H

#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/game_map.h"
#include "../common/game_snapshot.h"

#include "camera.h"
#include "my_player.h"

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

public:
    SDLManager();

    double calculate_angle_to_mouse(int mouse_x, int mouse_y) const;
    void clear_display();
    void show_screen();
    void update_camera(int player_x, int player_y);
    void render_in_z_order(const GameMap& map, const Snapshot& snapshot,
                           const std::string& my_username);
    void texto_prueba();
};

#endif
