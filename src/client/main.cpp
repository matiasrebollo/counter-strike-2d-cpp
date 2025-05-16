#include <QApplication>
#include <cmath>
#include <exception>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "client/lobby/lobby.h"

// #define DATA_PATH "/home/matias/Escritorio/TALLER 1/TP-GRUPAL/cs-2d-grupo-5-2025c1/assets/gfx"

int main(int argc, char* argv[]) try {
    QApplication q(argc, argv);
    Lobby l;
    l.show();
    q.exec();

    // Initialize SDL library
    SDL2pp::SDL sdl(SDL_INIT_VIDEO);

    SDL2pp::Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                          SDL_WINDOW_RESIZABLE);

    SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL2pp::Surface spriteSheet("../assets/gfx/player/ct1.bmp");
    SDL2pp::Texture sprite(renderer, spriteSheet);

    float x_pos = (renderer.GetOutputWidth() - 32) / 2;
    float y_pos = (renderer.GetOutputHeight() - 32) / 2;

    bool w = false, a = false, s = false, d = false;

    unsigned int prev_ticks = SDL_GetTicks();
    while (true) {
        unsigned int frame_ticks = SDL_GetTicks();
        unsigned int frame_delta = frame_ticks - prev_ticks;
        prev_ticks = frame_ticks;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        return 0;
                    case SDLK_w:
                        w = true;
                        break;
                    case SDLK_a:
                        a = true;
                        break;
                    case SDLK_s:
                        s = true;
                        break;
                    case SDLK_d:
                        d = true;
                        break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        w = false;
                        break;
                    case SDLK_a:
                        a = false;
                        break;
                    case SDLK_s:
                        s = false;
                        break;
                    case SDLK_d:
                        d = false;
                        break;
                }
            }
        }

        float movimiento = frame_delta * 0.2f;

        if (w && y_pos > 0)
            y_pos -= movimiento;
        if (s && y_pos + 32 < renderer.GetOutputHeight())
            y_pos += movimiento;
        if (a && x_pos > 0)
            x_pos -= movimiento;
        if (d && x_pos + 32 < renderer.GetOutputWidth())
            x_pos += movimiento;

        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        float dx = mouse_x - x_pos;
        float dy = mouse_y - y_pos;
        float ang_radianes = atan2(dy, dx);
        double angulo = (ang_radianes * 180.0f / M_PI) + 90;

        renderer.Clear();

        renderer.Copy(sprite, SDL2pp::Rect(0, 32, 32, 32),
                      SDL2pp::Rect((int)x_pos, (int)y_pos, 32, 32), angulo,
                      SDL2pp::Point(16.0f, 16.0f));

        renderer.Present();
        SDL_Delay(1);
    }

    return 0;
} catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
}
