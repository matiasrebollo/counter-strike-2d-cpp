#include "SDLManager.h"

#include <algorithm>

SDLManager::SDLManager():
        sdl(SDL_INIT_VIDEO),
        window("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_HEIGHT,
               WINDOW_WIDTH, SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        playerSheet("../assets/gfx/player/ct1.bmp"),
        player(renderer, playerSheet),
        boxSheet("../assets/gfx/tiles/aztec.bmp"),
        box(renderer, boxSheet),
        camera(CAMERA_HEIGHT, CAMERA_WIDTH) {}

std::pair<int, int> SDLManager::get_window_size() const {
    return {window.GetWidth(), window.GetHeight()};
}


void SDLManager::clear_display() { renderer.Clear(); }

void SDLManager::update_camera(int player_x, int player_y) {
    camera.follow(player_x + SIZE_PLAYER / 2, player_y + SIZE_PLAYER / 2);
}

float SDLManager::get_uniform_scale() const {
    float scale_x =
            static_cast<float>(window.GetWidth()) / static_cast<float>(camera.get_base_width());
    float scale_y =
            static_cast<float>(window.GetHeight()) / static_cast<float>(camera.get_base_height());
    return std::min(scale_x, scale_y);
}

SDL2pp::Point SDLManager::get_render_offset() const {
    float scale = get_uniform_scale();
    int render_w = static_cast<int>(camera.get_base_width() * scale);
    int render_h = static_cast<int>(camera.get_base_height() * scale);
    int offset_x = (window.GetWidth() - render_w) / 2;
    int offset_y = (window.GetHeight() - render_h) / 2;
    return SDL2pp::Point(offset_x, offset_y);
}

void SDLManager::render_in_z_order(const GameMap& map, const Snapshot& snapshot,
                                   const std::string& my_username) {

    for (const PlayerDTO& p: snapshot.players) {
        if (p.username == my_username) {
            update_camera(p.position.x, p.position.y);  // podria usar la de my_player
            // tengo q comentarlo por precommit break;
        }
    }

    float scale = get_uniform_scale();
    SDL2pp::Point offset = get_render_offset();
    int screen_width = static_cast<int>(camera.get_base_width() * scale);
    int screen_height = static_cast<int>(camera.get_base_height() * scale);
    SDL2pp::Rect viewport_rect(offset.GetX(), offset.GetY(), screen_width, screen_height);
    renderer.SetViewport(viewport_rect);


    // ACA SI ITERO EL MAPA (POR AHORA SOLO TIPO BOX)
    for (const MapObject& obj: map.map_objects) {
        if (obj.type == MapObjectType::BOX) {
            SDL2pp::Rect rect_origen(BOX_X_POS_SPRITE, BOX_Y_POS_SPRITE, SIZE_BOX, SIZE_BOX);
            SDL2pp::Rect destino_mundo(obj.position.x, obj.position.y, obj.width, obj.height);
            if (!camera.is_visible(destino_mundo))
                continue;
            SDL2pp::Rect destino_camera = camera.world_to_screen(destino_mundo);
            SDL2pp::Rect destino_scaled(
                    destino_camera.GetX() * scale, destino_camera.GetY() * scale,
                    destino_camera.GetW() * scale, destino_camera.GetH() * scale);
            renderer.Copy(box, rect_origen, destino_scaled);
        }
    }

    for (const PlayerDTO& p: snapshot.players) {
        double angulo = p.orientation;
        int x_pos = p.position.x;
        int y_pos = p.position.y;

        SDL2pp::Rect rect_origen(PLAYER_X_POS_SPRITE, PLAYER_Y_POS_SPRITE, SIZE_PLAYER,
                                 SIZE_PLAYER);
        SDL2pp::Rect destino_mundo(x_pos, y_pos, SIZE_PLAYER, SIZE_PLAYER);
        if (!camera.is_visible(destino_mundo))
            continue;
        SDL2pp::Rect destino_camera = camera.world_to_screen(destino_mundo);
        SDL2pp::Rect destino_scaled(destino_camera.GetX() * scale, destino_camera.GetY() * scale,
                                    destino_camera.GetW() * scale, destino_camera.GetH() * scale);
        SDL2pp::Point centro(static_cast<int>((SIZE_PLAYER / 2.0f) * scale),
                             static_cast<int>((SIZE_PLAYER / 2.0f) * scale));

        renderer.Copy(this->player, rect_origen, destino_scaled, angulo, centro);
    }
    renderer.SetViewport();
}

void SDLManager::show_screen() { renderer.Present(); }

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
