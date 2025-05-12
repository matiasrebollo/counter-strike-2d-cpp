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

    int x_center = renderer.GetOutputWidth() / 2;
    int y_center = renderer.GetOutputHeight() / 2;

    // Clear screen
    renderer.Clear();

    renderer.Copy(sprite,
                  Rect(0, 32, 32, 32),
                  Rect(x_center - 16, y_center - 16, 32, 32));

    // Show rendered frame
    renderer.Present();

    // 5 second delay
    SDL_Delay(5000);

    // Here all resources are automatically released and library deinitialized
    return 0;
} catch (std::exception& e) {
    // If case of error, print it and exit with error
    std::cerr << e.what() << std::endl;
    return 1;
}
