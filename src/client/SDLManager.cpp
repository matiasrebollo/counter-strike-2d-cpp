#include "SDLManager.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <vector>


SDLManager::SDLManager():
        sdl(SDL_INIT_VIDEO),
        window("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_INITIAL_WIDTH,
               WINDOW_INITIAL_HEIGHT, SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        playerSheet("../assets/gfx/player/ct1.bmp"),
        player(renderer, playerSheet),
        boxSheet("../assets/gfx/tiles/aztec.bmp"),
        box(renderer, boxSheet),
        hudNumbersSheet("../assets/gfx/hud_nums.bmp"),
        hudNumbers(renderer, hudNumbersSheet),
        camera(CAMERA_WIDTH, CAMERA_HEIGHT) {}

void SDLManager::render_waiting_screen(int players_connected, int players_required,
                                       const std::string& gamename, int iteration, int FPS) {

    // si sacamos todas las escalas, el texto no se agranda.
    auto [scale_x, scale_y] = get_scales();

    int large_font_size = 40;
    int small_font_size = 20;

    SDL2pp::SDLTTF ttf;
    SDL2pp::Font largeFont("../assets/cs_regular.ttf", large_font_size);
    SDL2pp::Font smallFont("../assets/cs_regular.ttf", small_font_size);

    int frames_per_dot = static_cast<int>(1.5f * FPS);
    int dots = (iteration / frames_per_dot) % 4;
    std::string waiting_text = "Waiting for players";
    waiting_text.append(dots, '.');

    // Texto waiting
    SDL2pp::Surface waitingSurface =
            largeFont.RenderText_Solid(waiting_text, SDL2pp::Color(255, 255, 255));
    SDL2pp::Texture waitingTexture(renderer, waitingSurface);
    int mainW = waitingSurface.GetWidth() * scale_x;
    int mainH = waitingSurface.GetHeight() * scale_y;
    SDL2pp::Rect waitingRect(static_cast<int>((WINDOW_INITIAL_WIDTH / 2) * scale_x - mainW / 2),
                             static_cast<int>((WINDOW_INITIAL_HEIGHT / 2) * scale_y - mainH / 2),
                             mainW, mainH);

    // Texto current players
    std::string players_text = std::to_string(players_connected) + "/" +
                               std::to_string(players_required) + " players connected";
    SDL2pp::Surface playersSurface =
            smallFont.RenderText_Solid(players_text, SDL2pp::Color(255, 255, 255));
    SDL2pp::Texture playersTexture(renderer, playersSurface);
    int playersW = playersSurface.GetWidth() * scale_x;
    int playersH = playersSurface.GetHeight() * scale_y;
    SDL2pp::Rect playersRect(static_cast<int>((WINDOW_INITIAL_WIDTH / 2) * scale_x - playersW / 2),
                             waitingRect.y + mainH + static_cast<int>(10 * scale_y), playersW,
                             playersH);

    // Texto gamename
    std::string gamename_text = "gamename: " + gamename;
    SDL2pp::Surface nameSurface =
            smallFont.RenderText_Solid(gamename_text, SDL2pp::Color(200, 200, 200));
    SDL2pp::Texture nameTexture(renderer, nameSurface);
    int nameW = nameSurface.GetWidth() * scale_x;
    int nameH = nameSurface.GetHeight() * scale_y;
    SDL2pp::Rect nameRect(static_cast<int>(WINDOW_INITIAL_WIDTH * scale_x) - nameW -
                                  static_cast<int>(10 * scale_x),
                          static_cast<int>(WINDOW_INITIAL_HEIGHT * scale_y) - nameH -
                                  static_cast<int>(10 * scale_y),
                          nameW, nameH);

    renderer.Copy(waitingTexture, SDL2pp::NullOpt, waitingRect);
    renderer.Copy(playersTexture, SDL2pp::NullOpt, playersRect);
    renderer.Copy(nameTexture, SDL2pp::NullOpt, nameRect);
}


std::pair<int, int> SDLManager::get_window_size() const {
    return {window.GetWidth(), window.GetHeight()};
}


void SDLManager::clear_display() { renderer.Clear(); }

void SDLManager::update_camera(int player_x, int player_y) {
    camera.follow(player_x + SIZE_PLAYER / 2, player_y + SIZE_PLAYER / 2);
}

std::pair<float, float> SDLManager::get_scales() const {
    float scale_x =
            static_cast<float>(window.GetWidth()) / static_cast<float>(WINDOW_INITIAL_WIDTH);
    float scale_y =
            static_cast<float>(window.GetHeight()) / static_cast<float>(WINDOW_INITIAL_HEIGHT);
    return {scale_x, scale_y};
}

float SDLManager::get_uniform_scale() const {
    auto [scale_x, scale_y] = get_scales();
    return std::min(scale_x, scale_y);
}


SDL2pp::Point SDLManager::get_render_offset() const {
    float scale = get_uniform_scale();
    int render_w = static_cast<int>(WINDOW_INITIAL_WIDTH * scale);
    int render_h = static_cast<int>(WINDOW_INITIAL_HEIGHT * scale);
    int offset_x = (window.GetWidth() - render_w) / 2;
    int offset_y = (window.GetHeight() - render_h) / 2;
    return SDL2pp::Point(offset_x, offset_y);
}

void SDLManager::render_player(const PlayerDTO& p, float scale) {
    double angulo = p.orientation;
    int x_pos = p.position.x;
    int y_pos = p.position.y;
    SDL2pp::Rect rect_origen(PLAYER_X_POS_SPRITE, PLAYER_Y_POS_SPRITE, SIZE_PLAYER, SIZE_PLAYER);
    SDL2pp::Rect destino_mundo(x_pos, y_pos, SIZE_PLAYER, SIZE_PLAYER);
    if (!camera.is_visible(destino_mundo))
        return;
    SDL2pp::Rect destino_camera = camera.world_to_screen(destino_mundo);
    SDL2pp::Rect destino_scaled(destino_camera.GetX() * scale, destino_camera.GetY() * scale,
                                destino_camera.GetW() * scale, destino_camera.GetH() * scale);
    SDL2pp::Point centro(static_cast<int>((SIZE_PLAYER / 2.0f) * scale),
                         static_cast<int>((SIZE_PLAYER / 2.0f) * scale));

    renderer.Copy(this->player, rect_origen, destino_scaled, angulo, centro);
}

void SDLManager::render_hud_time(int time_left, float scale) {
    int minutes = time_left / 60;
    int seconds = time_left % 60;

    std::stringstream ss;
    ss << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;
    std::string time_str = ss.str();

    int char_width = 48;
    int char_height = 66;
    int dp_width = 10;
    int spacing = 2;

    // porque me lo pide los linters
    int total_width = std::accumulate(time_str.begin(), time_str.end(), 0,
                                      [char_width, dp_width, spacing](int sum, char c) {
                                          return sum + (c == ':' ? dp_width : char_width) + spacing;
                                      });
    total_width -= spacing;

    int start_x = (WINDOW_INITIAL_WIDTH - total_width) / 2;
    int y = WINDOW_INITIAL_HEIGHT - char_height;

    hudNumbers.SetColorMod(255, 255, 0);
    hudNumbers.SetAlphaMod(160);

    for (size_t i = 0; i < time_str.size(); ++i) {
        char c = time_str[i];
        int index = 0;
        int width = char_width;
        if (std::isdigit(c)) {
            index = c - '0';
        } else if (c == ':') {
            index = 10;
            width = dp_width;
        } else {
            continue;
        }

        SDL2pp::Rect src(index * char_width, 0, width, char_height);
        SDL2pp::Rect dst((start_x + i * (width + spacing)) * scale, y * scale, width * scale,
                         char_height * scale);

        renderer.Copy(hudNumbers, src, dst);
    }
}


void SDLManager::render_in_z_order(const GameMap& map, const Snapshot& snapshot,
                                   const std::string& my_username) {

    for (const PlayerDTO& p: snapshot.ct) {
        if (p.username == my_username) {
            update_camera(p.position.x, p.position.y);
        }
    }
    for (const PlayerDTO& p: snapshot.tt) {
        if (p.username == my_username) {
            update_camera(p.position.x, p.position.y);
        }
    }

    float scale = get_uniform_scale();
    SDL2pp::Point offset = get_render_offset();
    int screen_width = static_cast<int>(WINDOW_INITIAL_WIDTH * scale);
    int screen_height = static_cast<int>(WINDOW_INITIAL_HEIGHT * scale);
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
    for (const PlayerDTO& p: snapshot.ct) {
        render_player(p, scale);
    }

    for (const PlayerDTO& p: snapshot.tt) {
        render_player(p, scale);
    }

    render_hud_time(snapshot.time_left, scale);
}

void SDLManager::show_screen() { renderer.Present(); }
