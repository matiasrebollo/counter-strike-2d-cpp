#include "SDLManager.h"

#include <string>

#include "../common/block_texture_parser.h"

SDLManager::SDLManager():
        sdl(SDL_INIT_VIDEO),
        window("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
               SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        playerSheet("../assets/gfx/player/ct1.bmp"),
        player(renderer, playerSheet),
        boxSheet("../assets/gfx/tiles/aztec.bmp"),
        box(renderer, boxSheet) {}

void SDLManager::clear_display() { renderer.Clear(); }

void SDLManager::show_screen() { renderer.Present(); }

void SDLManager::render_in_z_order(const GameMap& map, const Snapshot& snapshot, MyPlayer& player) {

    // ACA SI ITERO EL MAPA (POR AHORA SOLO TIPO BOX)
    BlockTextureParser texture_parser;
    for (const MapObject& obj: map.map_objects) {
        BlockTextureInfo txt = texture_parser.get_texture_info(obj.type);
        std::string path = "../assets/gfx/tiles/" + txt.tileset_path;

        SDL2pp::Surface boxSheet2(path);
        SDL2pp::Texture box2(renderer, boxSheet2);

        SDL2pp::Rect rect_origen(txt.x, txt.y, txt.width, txt.height);
        for (const auto& vec: obj.positions) {
            SDL2pp::Rect rect_destino(vec.x * 32, vec.y * 32, 32, 32);
            renderer.Copy(box2, rect_origen, rect_destino);
        }
        /*if (obj.type == MapObjectType::BOX) {
            SDL2pp::Rect rect_origen(416, 64, 32, 32);  // por ahora lo hardcodeo
            SDL2pp::Rect rect_destino(obj.position.x, obj.position.y,32,32);
            renderer.Copy(box, rect_origen, rect_destino);
        }*/
    }

    for (const PlayerDTO& p: snapshot.players) {
        player.update_my_position(p);
        double angulo = p.orientation;
        int x_pos = p.position.x;
        int y_pos = p.position.y;

        SDL2pp::Rect rect_origen(0, 32, 32, 32);
        SDL2pp::Rect rect_destino(x_pos, y_pos, 32, 32);
        SDL2pp::Point centro(16, 16);

        renderer.Copy(this->player, rect_origen, rect_destino, angulo, centro);
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
