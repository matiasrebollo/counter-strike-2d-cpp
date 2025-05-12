#include <exception>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "common/foo.h"

using namespace SDL2pp;

#define DATA_PATH "/home/matias/Escritorio/TALLER 1/TP-GRUPAL/cs-2d-grupo-5-2025c1/assets/gfx"

int main() try {
    // Initialize SDL library
    SDL sdl(SDL_INIT_VIDEO);

    // Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
    Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                  SDL_WINDOW_RESIZABLE);

    // Create accelerated video renderer with default driver
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    Surface spriteSheet(DATA_PATH "/player/ct1.bmp");
    Texture sprite(renderer, spriteSheet);

    float x_pos = (renderer.GetOutputWidth() - 32) / 2;
    float y_pos = (renderer.GetOutputHeight() - 32) / 2;

    bool w = false, a = false, s = false, d = false;

    unsigned int prev_ticks = SDL_GetTicks();
    while (true) {
        unsigned int frame_ticks = SDL_GetTicks();
        unsigned int frame_delta = frame_ticks - prev_ticks;
        prev_ticks = frame_ticks;

        SDL_Event event;
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                return 0;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: return 0;
                    case SDLK_w: w = true; break;
                    case SDLK_a: a = true; break;
                    case SDLK_s: s = true; break;
                    case SDLK_d: d = true; break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch(event.key.keysym.sym) {
                    case SDLK_w: w = false; break;
                    case SDLK_a: a = false; break;
                    case SDLK_s: s = false; break;
                    case SDLK_d: d = false; break;
                }
            } 
        }

        float movimiento = frame_delta * 0.2f;
        if (w && y_pos > 0){
            y_pos -= movimiento;
        }
        if (s && y_pos + 32 < renderer.GetOutputHeight()) {
            y_pos += movimiento;
        }
        if (a && x_pos > 0){
            x_pos -= movimiento;
        }
        if (d && x_pos + 32 < renderer.GetOutputWidth()) {
            x_pos += movimiento;
        }

        // Clear screen
        renderer.Clear();

        renderer.Copy(sprite,
                  Rect(0, 32, 32, 32),
                  Rect((int)x_pos, (int)y_pos, 32, 32));

        // Show rendered frame
        renderer.Present();

        // Duerme un poquito para no quemar el CPU
        SDL_Delay(1);
    }


    // Here all resources are automatically released and library deinitialized
    return 0;
} catch (std::exception& e) {
    // If case of error, print it and exit with error
    std::cerr << e.what() << std::endl;
    return 1;
}
