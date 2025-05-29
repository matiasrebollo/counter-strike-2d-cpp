#include "SDLManager.h"

SDLManager::SDLManager():
        sdl(SDL_INIT_VIDEO),
        window("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240,
               SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        playerSheet("../assets/gfx/player/ct1.bmp"),
        player(renderer, playerSheet),
        boxSheet("../assets/gfx/tiles/aztec.bmp"),
        box(renderer, boxSheet),
        camera(320, 240) {}


double SDLManager::calculate_angle_to_mouse(int mouse_x, int mouse_y) const {
    return camera.calculate_angle_from_center(mouse_x, mouse_y);
}

void SDLManager::clear_display() { renderer.Clear(); }

void SDLManager::show_screen() { renderer.Present(); }

void SDLManager::update_camera(int player_x, int player_y) { camera.follow(player_x, player_y); }

void SDLManager::render_in_z_order(const GameMap& map, const Snapshot& snapshot,
                                   const std::string& my_username) {

    // ACA SI ITERO EL MAPA (POR AHORA SOLO TIPO BOX)
    for (const MapObject& obj: map.map_objects) {
        if (obj.type == MapObjectType::BOX) {
            SDL2pp::Rect rect_origen(416, 64, 32, 32);  // por ahora lo hardcodeo
            SDL2pp::Rect destino_mundo(obj.position.x, obj.position.y, obj.width, obj.height);
            SDL2pp::Rect destino_camera = camera.world_to_screen(destino_mundo);
            renderer.Copy(box, rect_origen, destino_camera);
        }
    }

    for (const PlayerDTO& p: snapshot.players) {
        if (p.username == my_username) {
            camera.follow(p.position.x + 16,
                          p.position.y + 16);  // centramos en el medio del sprite (32x32)
            // break;
        }
    }

    for (const PlayerDTO& p: snapshot.players) {
        double angulo = p.orientation;
        int x_pos = p.position.x;
        int y_pos = p.position.y;

        SDL2pp::Rect rect_origen(0, 32, 32, 32);
        SDL2pp::Rect destino_mundo(x_pos, y_pos, 32, 32);
        SDL2pp::Rect destino_camera = camera.world_to_screen(destino_mundo);
        SDL2pp::Point centro(16, 16);

        renderer.Copy(this->player, rect_origen, destino_camera, angulo, centro);
    }
}

void SDLManager::texto_prueba() {
    // Texto hardcodeado por ahora, luego hay que borrarlo
    SDL2pp::SDLTTF ttf;
    SDL2pp::Font font("../assets/gfx/fonts/sourcesans.ttf", 24);
    SDL2pp::Surface textSurface =
            font.RenderText_Solid("Waiting for players...", SDL2pp::Color(255, 255, 255));
    SDL2pp::Texture textTexture(renderer, textSurface);
    renderer.Clear();
    int textW = textSurface.GetWidth();
    int textH = textSurface.GetHeight();
    SDL2pp::Rect dstRect(210, 210, textW, textH);
    renderer.Copy(textTexture, SDL2pp::NullOpt, dstRect);
    renderer.Present();
}
