#include "client.h"
#include "client/lobby/lobby.h"
#include "../common/game_map.h"

#include <QApplication>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>

Client::Client() {}

void Client::run(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Lobby lobby;
    lobby.show();
    app.exec();


    SDL2pp::SDL sdl(SDL_INIT_VIDEO);

    SDL2pp::Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000,
                          SDL_WINDOW_RESIZABLE);

    SDL2pp::Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    //Jugador
    SDL2pp::Surface playerSheet("../assets/gfx/player/ct1.bmp");
    SDL2pp::Texture player(renderer, playerSheet);

    //Caja
    SDL2pp::Surface boxSheet("../assets/gfx/tiles/aztec.bmp");
    SDL2pp::Texture box(renderer, boxSheet);

    float x_pos, y_pos;
    bool primeraVez = true;

    bool w = false, a = false, s = false, d = false;

    // ESTA HARDCODEADA DE OBJETOS CUANDO ESTEN LOS SNAPSHOTS SE SACA
    const int mapWidth = 1000;
    const int mapHeight = 1000;
    const int wallThickness = 100;
    const int boxThickness = 100;

    std::vector<MapObject> objects;

    Vector2D pos(0, 0);
    int width = mapWidth;
    int height = wallThickness;
    MapObject obj{pos, width, height, MapObjectType::BOX};
    objects.push_back(obj);

    pos = Vector2D(0, 0);
    width = wallThickness;
    height = mapHeight;
    MapObject obj2{pos, width, height, MapObjectType::BOX};
    objects.push_back(obj2);

    pos = Vector2D(0, mapHeight - wallThickness);
    width = mapWidth;
    height = wallThickness;
    MapObject obj3{pos, width, height, MapObjectType::BOX};
    objects.push_back(obj3);

    pos = Vector2D(mapWidth - wallThickness, 0);
    width = wallThickness;
    height = mapHeight;
    MapObject obj4{pos, width, height, MapObjectType::BOX};
    objects.push_back(obj4);

    pos = Vector2D(450, 450);
    width = boxThickness;
    height = boxThickness;
    MapObject obj5{pos, width, height, MapObjectType::BOX};
    objects.push_back(obj5);

    const GameMap map{objects};

    unsigned int prev_ticks = SDL_GetTicks();
    while (true) {
        unsigned int frame_ticks = SDL_GetTicks();
        unsigned int frame_delta = frame_ticks - prev_ticks;
        prev_ticks = frame_ticks;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return;
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: return;
                    case SDLK_w: w = true; break;
                    case SDLK_a: a = true; break;
                    case SDLK_s: s = true; break;
                    case SDLK_d: d = true; break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_w: w = false; break;
                    case SDLK_a: a = false; break;
                    case SDLK_s: s = false; break;
                    case SDLK_d: d = false; break;
                }
            }
        }

        // Primer frame 
        if (primeraVez) {
            x_pos = (renderer.GetOutputWidth() - 32) / 2;
            y_pos = (renderer.GetOutputHeight() - 32) / 2;
            primeraVez = false;
        }

        float movimiento = frame_delta * 0.2f;

        if (w && y_pos > 0) y_pos -= movimiento;
        if (s && y_pos + 32 < renderer.GetOutputHeight()) y_pos += movimiento;
        if (a && x_pos > 0) x_pos -= movimiento;
        if (d && x_pos + 32 < renderer.GetOutputWidth()) x_pos += movimiento;

        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        float dx = mouse_x - x_pos;
        float dy = mouse_y - y_pos;
        float ang_radianes = atan2(dy, dx);
        double angulo = (ang_radianes * 180.0f / M_PI) + 90;

        renderer.Clear();

        // ACA SI ITERO EL MAPA (POR AHORA SOLO TIPO BOX)
        for (const MapObject& obj : map.map_objects) {
            if (obj.type == MapObjectType::BOX) {
                SDL2pp::Rect rect_origen(416, 64, 32, 32); //por ahora lo hardcodeo
                SDL2pp::Rect rect_destino(obj.position.x, obj.position.y, obj.width, obj.height);
                renderer.Copy(box,
                              rect_origen,
                              rect_destino);
            }
        }
        // podria tambien crear los rect y point antes en lugar de en el copy
        renderer.Copy(player, SDL2pp::Rect(0, 32, 32, 32),
                      SDL2pp::Rect((int)x_pos, (int)y_pos, 32, 32),
                      angulo, SDL2pp::Point(16.0f, 16.0f));
        renderer.Present();
        SDL_Delay(1);
    }
}
