#ifndef SDLMANAGER_H
#define SDLMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/game_map.h"
#include "../common/game_snapshot.h"

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

public:
    SDLManager();

    void clear_display();
    void show_screen();
    void render_in_z_order(const GameMap& map, const Snapshot& snapshot);
    void texto_prueba();
};

#endif
