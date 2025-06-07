#include "SDLManager.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include "../common/block_texture_parser.h"


SDLManager::SDLManager():
        sdl(SDL_INIT_VIDEO),
        window("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_INITIAL_WIDTH,
               WINDOW_INITIAL_HEIGHT, SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        texture_manager(renderer),
        camera(CAMERA_WIDTH, CAMERA_HEIGHT),
        shop(renderer, texture_manager, texture_parser) {
    renderer.SetLogicalSize(CAMERA_WIDTH, CAMERA_HEIGHT);
}  // para no hacerlo cada frame

void SDLManager::render_waiting_screen(int players_connected, int players_required,
                                       const std::string& gamename, int iteration, int FPS) {

    // si font_scale es menor a 1 queda medio mal
    int large_font_size = 53;
    int small_font_size = 27;
    // Fondo

    const std::string& font_path = texture_parser.get_fw_texture(FONT_WAITING);
    const std::string& background_path = texture_parser.get_fw_texture(BACKGROUND);

    SDL2pp::Texture& background = texture_manager.get_texture(background_path);
    SDL2pp::Rect backgroundRect(0, 0, CAMERA_WIDTH, CAMERA_HEIGHT);
    renderer.Copy(background, SDL2pp::NullOpt, backgroundRect);

    // ver forma de no recibir FPS
    int frames_per_dot = static_cast<int>(1.5f * FPS);
    int dots = (iteration / frames_per_dot) % 4;
    std::string waiting_text = "Waiting for players";
    waiting_text.append(dots, '.');

    // Texto waiting
    SDL2pp::Texture& waitingTexture = texture_manager.get_text_texture(
            waiting_text, font_path, large_font_size, SDL2pp::Color(255, 255, 255));
    int mainW = waitingTexture.GetWidth();
    int mainH = waitingTexture.GetHeight();
    SDL2pp::Rect waitingRect((CAMERA_WIDTH / 2) - mainW / 2, (CAMERA_HEIGHT / 2) - mainH / 2, mainW,
                             mainH);

    // Texto current players
    std::string players_text = std::to_string(players_connected) + "/" +
                               std::to_string(players_required) + " players connected";
    SDL2pp::Texture& playersTexture = texture_manager.get_text_texture(
            players_text, font_path, small_font_size, SDL2pp::Color(255, 255, 255));
    int playersW = playersTexture.GetWidth();
    int playersH = playersTexture.GetHeight();
    SDL2pp::Rect playersRect((CAMERA_WIDTH / 2) - playersW / 2, waitingRect.y + mainH + 10,
                             playersW, playersH);

    // Texto gamename
    std::string gamename_text = "gamename: " + gamename;
    SDL2pp::Texture& nameTexture = texture_manager.get_text_texture(
            gamename_text, font_path, small_font_size, SDL2pp::Color(200, 200, 200));
    int nameW = nameTexture.GetWidth();
    int nameH = nameTexture.GetHeight();
    SDL2pp::Rect nameRect(CAMERA_WIDTH - nameW - 10, CAMERA_HEIGHT - nameH - 10, nameW, nameH);

    renderer.Copy(waitingTexture, SDL2pp::NullOpt, waitingRect);
    renderer.Copy(playersTexture, SDL2pp::NullOpt, playersRect);
    renderer.Copy(nameTexture, SDL2pp::NullOpt, nameRect);
}

SDL_Point SDLManager::get_logical_size() const { return renderer.GetLogicalSize(); }

void SDLManager::clear_display() { renderer.Clear(); }

void SDLManager::update_camera(int player_x, int player_y) {
    camera.follow(player_x + SIZE_PLAYER / 2, player_y + SIZE_PLAYER / 2);
}

void SDLManager::render_player(const PlayerDTO& p, const BlockTextureInfo& sprite_info) {
    double angulo = p.orientation;
    int x_pos = p.position.x;
    int y_pos = p.position.y;

    SDL2pp::Rect rect_origen(sprite_info.x, sprite_info.y, sprite_info.width, sprite_info.height);
    SDL2pp::Rect destino_mundo(x_pos, y_pos, SIZE_PLAYER, SIZE_PLAYER);

    if (!camera.is_visible(destino_mundo))
        return;

    SDL2pp::Rect destino_camera = camera.world_to_screen(destino_mundo);
    SDL2pp::Point centro(SIZE_PLAYER / 2, SIZE_PLAYER / 2);

    std::string path = sprite_info.tileset_path;

    SDL2pp::Texture& skin_texture = texture_manager.get_texture(path);

    renderer.Copy(skin_texture, rect_origen, destino_camera, angulo, centro);
}


void SDLManager::render_hud_time(int time_left) {
    int minutes = time_left / 60;
    int seconds = time_left % 60;

    std::stringstream ss;
    ss << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;
    std::string time_str = ss.str();

    int clock_width = 30, clock_height = 33;
    int char_width = 24;
    int char_height = 33;
    int dp_width = 5;
    int spacing = 2;


    // porque me lo pide los linters
    int text_width = std::accumulate(time_str.begin(), time_str.end(), 0,
                                     [char_width, dp_width, spacing](int sum, char c) {
                                         return sum + (c == ':' ? dp_width : char_width) + spacing;
                                     }) -
                     spacing;
    int total_width = clock_width + spacing + text_width;

    int start_x = (CAMERA_WIDTH - total_width) / 2;
    int y = CAMERA_HEIGHT - char_height;

    const BlockTextureInfo& clock_info = texture_parser.get_symbol_texture(CLOCK);
    SDL2pp::Texture& clock_texture = texture_manager.get_texture(clock_info.tileset_path);
    clock_texture.SetColorMod(255, 255, 0);
    clock_texture.SetAlphaMod(190);

    SDL2pp::Rect clock_src(clock_info.x, clock_info.y, clock_info.width, clock_info.height);
    SDL2pp::Rect clock_dst(start_x, y, clock_width, clock_height);
    renderer.Copy(clock_texture, clock_src, clock_dst);

    int x = start_x + clock_width + spacing;
    for (char c: time_str) {
        HudNumbers num_enum = (c == ':') ? DP : static_cast<HudNumbers>(c - '0');
        const BlockTextureInfo& sprite_info = texture_parser.get_number_texture(num_enum);
        SDL2pp::Texture& texture = texture_manager.get_texture(sprite_info.tileset_path);
        texture.SetColorMod(255, 255, 0);
        texture.SetAlphaMod(190);

        int width = char_width;
        if (c == ':') {
            width = dp_width;
        }

        SDL2pp::Rect src(sprite_info.x, sprite_info.y, sprite_info.width, sprite_info.height);
        SDL2pp::Rect dst(x, y, width, char_height);

        renderer.Copy(texture, src, dst);

        x += width + spacing;
    }
}

void SDLManager::render_hud_life(uint16_t life) {

    int plus_width = 30, plus_height = 33;

    int char_width = 24;
    int char_height = 33;
    int spacing = 2;

    std::string life_str = std::to_string(life);

    int start_x = 0;
    int y = CAMERA_HEIGHT - char_height;

    const BlockTextureInfo& life_info = texture_parser.get_symbol_texture(HEALTH);
    SDL2pp::Texture& life_texture = texture_manager.get_texture(life_info.tileset_path);
    life_texture.SetColorMod(255, 255, 0);
    life_texture.SetAlphaMod(190);

    SDL2pp::Rect src_icon(life_info.x, life_info.y, life_info.width, life_info.height);
    SDL2pp::Rect dst_icon(start_x, y, plus_width, plus_height);
    renderer.Copy(life_texture, src_icon, dst_icon);

    int x = start_x + plus_width + spacing;
    for (char c: life_str) {
        HudNumbers num_enum = static_cast<HudNumbers>(c - '0');
        const BlockTextureInfo& sprite_info = texture_parser.get_number_texture(num_enum);
        SDL2pp::Texture& texture = texture_manager.get_texture(sprite_info.tileset_path);

        texture.SetColorMod(255, 255, 0);
        texture.SetAlphaMod(190);

        SDL2pp::Rect src(sprite_info.x, sprite_info.y, sprite_info.width, sprite_info.height);
        SDL2pp::Rect dst(x, y, char_width, char_height);

        renderer.Copy(texture, src, dst);

        x += char_width + spacing;
    }
}

void SDLManager::render_in_z_order(const GameMap& map, const Snapshot& snapshot,
                                   const LocalPlayerInfo& local_info) {

    for (const PlayerDTO& p: snapshot.ct) {
        if (p.username == local_info.username) {
            update_camera(p.position.x, p.position.y);
        }
    }
    for (const PlayerDTO& p: snapshot.tt) {
        if (p.username == local_info.username) {
            update_camera(p.position.x, p.position.y);
        }
    }

    for (const MapObject& obj: map.map_objects) {
        const BlockTextureInfo& obj_info = texture_parser.get_texture_info(obj.type);
        std::string path = obj_info.tileset_path;

        SDL2pp::Texture& obj_texture = texture_manager.get_texture(path);

        SDL2pp::Rect rect_origen(obj_info.x, obj_info.y, obj_info.width, obj_info.height);
        for (const auto& vec: obj.positions) {
            SDL2pp::Rect destino_mundo(vec.x * 40, vec.y * 40, 40, 40);
            if (!camera.is_visible(destino_mundo))
                continue;
            SDL2pp::Rect destino_camera = camera.world_to_screen(destino_mundo);
            renderer.Copy(obj_texture, rect_origen, destino_camera);
        }
    }

    for (const PlayerDTO& p: snapshot.ct) {
        const BlockTextureInfo& skin_info =
                texture_parser.get_ct_texture(local_info.ct_skin, CARRY_KNIFE);
        render_player(p, skin_info);
    }

    for (const PlayerDTO& p: snapshot.tt) {
        const BlockTextureInfo& skin_info =
                texture_parser.get_tt_texture(local_info.tt_skin, CARRY_KNIFE);
        render_player(p, skin_info);
    }

    render_hud_time(snapshot.time_left);
    for (const PlayerDTO& p: snapshot.ct) {
        if (p.username == local_info.username) {
            render_hud_life(p.life);
        }
    }
    for (const PlayerDTO& p: snapshot.tt) {
        if (p.username == local_info.username) {
            render_hud_life(p.life);
        }
    }
}

std::optional<ShopButtonType> SDLManager::get_clicked_button(int x, int y) {
    return shop.clicked_button(x, y);
}


void SDLManager::render_shop() { shop.render(); }

void SDLManager::show_screen() { renderer.Present(); }
