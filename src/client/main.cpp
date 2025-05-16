#include <exception>
#include <iostream>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <QApplication>

using namespace SDL2pp;
#include "common/foo.h"
#include "lobby/lobby.h"

#define DATA_PATH "/home/matias/Escritorio/TALLER 1/TP-GRUPAL/cs-2d-grupo-5-2025c1/assets/gfx" //deberia cambiarlo

int main(int argc, char* argv[]) try {

    QApplication a(argc, argv);
    Lobby w;
    w.show();
    a.exec();

    // Initialize SDL library
    SDL sdl(SDL_INIT_VIDEO);

    /* Creamos la ventana de 640x480 (puede cambiar) y posicionada automaticamente */
    Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                  SDL_WINDOW_RESIZABLE);

    /* Creamos el renderer que se encarga de dibujar en window */
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    Surface spriteSheet(DATA_PATH "/player/ct1.bmp");
    Texture sprite(renderer, spriteSheet);

    /* Comenzamos en el centro */
    float x_pos = (renderer.GetOutputWidth() - 32) / 2;
    float y_pos = (renderer.GetOutputHeight() - 32) / 2;

    /* Booleanos para ver que tecla se toca */
    bool w = false, a = false, s = false, d = false;

    /* Nos guardamos el tiempo en ms desde que inicializamos sdl */
    unsigned int prev_ticks = SDL_GetTicks();
    while (true) {
        /* Nos guardamos la diferencia de ms entre este frame y el anterior */
        unsigned int frame_ticks = SDL_GetTicks();
        unsigned int frame_delta = frame_ticks - prev_ticks;
        prev_ticks = frame_ticks;

        /* Procesamos el evento */
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            /* Si se cierra la ventana salimos del juego */
            if (event.type == SDL_QUIT){
                return 0;
            /* Si se toca una tecla activamos su booleano o si es Esc cerramos el juego */
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: return 0;
                    case SDLK_w: w = true; break;
                    case SDLK_a: a = true; break;
                    case SDLK_s: s = true; break;
                    case SDLK_d: d = true; break;
                }
            /* Si se suelta una tecla desactivamos su booleano */
            } else if (event.type == SDL_KEYUP) {
                switch(event.key.keysym.sym) {
                    case SDLK_w: w = false; break;
                    case SDLK_a: a = false; break;
                    case SDLK_s: s = false; break;
                    case SDLK_d: d = false; break;
                }
            } 
        }

        /* Determina cuanto se mueve el personaje en este frame basado en cuanto tiempo paso */
        float movimiento = frame_delta * 0.2f;

        /* Actualizamos posicion segun corresponda sin pasarnos de los bordes */
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

        /* Obtenemos posicion del mouse */
        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        /* Calculamos angulo entre el personaje y el cursor */
        float dx = mouse_x - x_pos;
        float dy = mouse_y - y_pos;
        float ang_radianes = atan2(dy, dx); //angulo en radianes
        double angulo = (ang_radianes * 180.0f / M_PI) + 90; //conversion de radianes a grados

        // Clear screen
        renderer.Clear();

        renderer.Copy(sprite,
                  Rect(0, 32, 32, 32),
                  Rect((int)x_pos, (int)y_pos, 32, 32),
                  angulo, 
                  Point(16.0f, 16.0f)); // para que rote al rededor del centro del sprite y no de su izq sup

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
