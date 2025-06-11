#include "SDLManager.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include "../common/block_texture_parser.h"

// Es una clase muy grande, quizas se pueda separar en subclases (como una para el HUD).

SDLManager::SDLManager():
        sdl(SDL_INIT_VIDEO),
        window("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_INITIAL_WIDTH,
               WINDOW_INITIAL_HEIGHT, SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        texture_manager(renderer),
        camera(CAMERA_WIDTH, CAMERA_HEIGHT),
        shop(renderer, texture_manager, texture_parser) {
    renderer.SetLogicalSize(CAMERA_WIDTH, CAMERA_HEIGHT);
    SDL_ShowCursor(SDL_DISABLE);
}

void SDLManager::set_map(GameMapDTO game_map) { map = std::move(game_map); }

void SDLManager::set_shop(ShopInfoDTO shop_info) {
    for (const auto& [gun, price]: shop_info.shop_gun_prices) {
        std::cout << gun << std::endl;
        std::cout << price << std::endl;
    }
    for (const auto& [gun, amount]: shop_info.shop_clip_by_gun_prices) {
        std::cout << gun << std::endl;
        std::cout << amount << std::endl;
    }
}

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

/* Centra la camara en el player */
void SDLManager::update_camera(int player_x, int player_y) {
    camera.follow(player_x + SIZE_PLAYER / 2, player_y + SIZE_PLAYER / 2);
}

/* Devuelve un pair de la posicion del player segun el arma equipada y el sprite del arma a usar */
std::pair<Position, GunSprites> SDLManager::get_gun_info(const LoadoutDTO& loadout) {
    switch (loadout.equipped) {
        case KNIFE:
            return {CARRY_KNIFE, KNIFE_GAME};
        // por ahora solo secondary glock
        case SECONDARY:
            return {CARRY_SECONDARY, GLOCK_GAME};
        case PRIMARY:
            switch (loadout.primary_gun) {
                case AK47:
                    return {CARRY_PRIMARY, AK47_GAME};
                case AWP:
                    return {CARRY_PRIMARY, AWP_GAME};
                case M3:
                    return {CARRY_PRIMARY, M3_GAME};
                default:
                    return {CARRY_PRIMARY,
                            AK47_GAME};  // aca llegamos en caso de que sea NONE, no deberia pasar.
            }
        /*case BOMB:
            return {CARRY_BOMB, BOMB_GAME};  */
        default:
            return {CARRY_KNIFE, KNIFE_GAME};
    }
}

/* Renderiza un jugador */
void SDLManager::render_player(const PlayerDTO& p, const BlockTextureInfo& sprite_info) {
    double angulo = p.orientation;
    int x_pos = p.position.x;
    int y_pos = p.position.y;

    SDL2pp::Rect rect_origen(sprite_info.x, sprite_info.y, sprite_info.width, sprite_info.height);
    SDL2pp::Rect destino_mundo(x_pos, y_pos, SIZE_PLAYER, SIZE_PLAYER);

    if (!camera.is_visible(destino_mundo))
        return;

    SDL2pp::Rect destino_camera = camera.world_to_screen(destino_mundo);
    std::string path = sprite_info.tileset_path;

    SDL2pp::Texture& skin_texture = texture_manager.get_texture(path);
    renderer.Copy(skin_texture, rect_origen, destino_camera, angulo, SDL2pp::NullOpt);
}

// falta hacer que quizas podes no ver el player pero si el arma (x la camera)
// no dibujo las armas junto a cada player para que todas las armas se dibujen sobre los demas
// players (z order)
/* Renderiza las armas de cada jugador */
void SDLManager::render_player_weapon(const PlayerDTO& p) {
    double angulo = p.orientation;
    int x_pos = p.position.x;
    int y_pos = p.position.y;

    SDL2pp::Rect destino_mundo(x_pos, y_pos, SIZE_PLAYER, SIZE_PLAYER);
    if (!camera.is_visible(destino_mundo))
        return;

    SDL2pp::Rect destino_camera = camera.world_to_screen(destino_mundo);
    GunSprites sprite = get_gun_info(p.loadout).second;
    std::string weapon_path = texture_parser.get_gun_texture(sprite);

    int offset_x, offset_y, gun_width, gun_height;
    if (sprite == KNIFE_GAME) {
        offset_x = 18;
        offset_y = -10;
        gun_width = 20;
        gun_height = 40;
        angulo -= 110;
    } else {
        offset_x = 0;
        offset_y = -17;
        gun_width = 32;
        gun_height = 32;
    }

    SDL2pp::Rect gun_dst(destino_camera.GetX() + offset_x, destino_camera.GetY() + offset_y,
                         gun_width, gun_height);

    SDL2pp::Point rotate(-offset_x + SIZE_PLAYER / 2, -offset_y + SIZE_PLAYER / 2);
    SDL2pp::Texture& weapon_texture = texture_manager.get_texture(weapon_path);

    renderer.Copy(weapon_texture, SDL2pp::NullOpt, gun_dst, angulo, rotate);
}

/* Renderiza el tiempo restante de la ronda del HUD */
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

/* Renderiza la vida del HUD */
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

// fijarse de quizas agregar un iconito
/* Renderiza la municion del HUD */
void SDLManager::render_hud_ammo(int ammo) {
    int char_width = 24;
    int char_height = 33;
    int spacing = 2;

    std::string ammo_str = std::to_string(ammo);

    int text_width = (ammo_str.size()) * (char_width + spacing) - spacing;

    int start_x = CAMERA_WIDTH - text_width;
    int y = CAMERA_HEIGHT - char_height;

    int x = start_x;
    for (char c: ammo_str) {
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

/* Renderiza el dinero del HUD */
void SDLManager::render_hud_money(int money) {
    int icon_width = 30;
    int icon_height = 33;

    int char_width = 24;
    int char_height = 33;
    int spacing = 2;

    int max_digits = 5;
    int total_text_width = max_digits * (char_width + spacing) - spacing;

    int text_start_x = CAMERA_WIDTH - total_text_width;

    int icon_x = text_start_x - icon_width - spacing;

    int y = CAMERA_HEIGHT - char_height * 2 - spacing;

    const BlockTextureInfo& icon_info = texture_parser.get_symbol_texture(MONEY);
    SDL2pp::Texture& icon_texture = texture_manager.get_texture(icon_info.tileset_path);
    icon_texture.SetColorMod(255, 255, 0);
    icon_texture.SetAlphaMod(190);

    SDL2pp::Rect src_icon(icon_info.x, icon_info.y, icon_info.width, icon_info.height);
    SDL2pp::Rect dst_icon(icon_x, y, icon_width, icon_height);
    renderer.Copy(icon_texture, src_icon, dst_icon);

    std::string money_str = std::to_string(money);

    int text_width = (money_str.size()) * (char_width + spacing) - spacing;
    int number_x = CAMERA_WIDTH - text_width;

    int x = number_x;
    for (char c: money_str) {
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

// quizas eliminar la snapshot y englobar localinfo en un gamestate y solo recibir gamestate
void SDLManager::render_in_z_order(const Snapshot& snapshot, const LocalInfo& local_info) {

    update_camera(local_info.x, local_info.y);

    if (map.has_value()) {
        GameMapDTO game_map = map.value();

        const std::string& background_path =
                texture_parser.get_background_path(game_map.background);
        SDL2pp::Texture& background = texture_manager.get_texture(background_path);
        SDL2pp::Rect backgroundRect(0, 0, CAMERA_WIDTH, CAMERA_HEIGHT);
        renderer.Copy(background, SDL2pp::NullOpt, backgroundRect);

        for (const MapObject& obj: game_map.map_objects) {
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
    }

    for (const PlayerDTO& p: snapshot.ct) {
        Position pos = get_gun_info(p.loadout).first;
        const BlockTextureInfo& skin_info = texture_parser.get_ct_texture(local_info.ct_skin, pos);
        render_player(p, skin_info);
    }

    for (const PlayerDTO& p: snapshot.tt) {
        Position pos = get_gun_info(p.loadout).first;
        const BlockTextureInfo& skin_info = texture_parser.get_tt_texture(local_info.tt_skin, pos);
        render_player(p, skin_info);
    }

    // renderizo las armas luego de los players para que las armas siempre aparezcan por encima de
    // estos, ver si modificar
    for (const PlayerDTO& p: snapshot.ct) {
        render_player_weapon(p);
    }

    for (const PlayerDTO& p: snapshot.tt) {
        render_player_weapon(p);
    }

    render_hud_time(snapshot.time_left);
    render_hud_life(local_info.life);
    render_hud_ammo(local_info.equipped_gun_ammo);
    render_hud_money(local_info.money);
}

std::optional<ShopButtonType> SDLManager::get_clicked_button(int x, int y) {
    return shop.clicked_button(x, y);
}


void SDLManager::render_shop(int player_money, GunType primary_gun, GunType secondary_gun) {
    shop.render(player_money, primary_gun, secondary_gun);
}

/* Devuelve el color de la mira a usar dependiendo donde esta posicionado el mouse */
Crosshairs SDLManager::get_crosshair_color(int mouse_x, int mouse_y, const Snapshot& snapshot,
                                           const LocalInfo& local_info) {

    const auto& enemies = local_info.is_ct ? snapshot.tt : snapshot.ct;

    for (const auto& e: enemies) {
        SDL2pp::Rect destino_mundo(e.position.x, e.position.y, SIZE_PLAYER, SIZE_PLAYER);

        if (!camera.is_visible(destino_mundo)) {
            continue;
        }

        SDL2pp::Rect destino_camera = camera.world_to_screen(destino_mundo);

        if (destino_camera.Contains(mouse_x, mouse_y)) {
            return RED;
        }
    }

    return GREEN;
}


void SDLManager::render_crosshair(const Snapshot& snapshot, const LocalInfo& local_info) {

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);  // da coords fisicas

    float logical_mouse_x, logical_mouse_y;
    SDL_RenderWindowToLogical(renderer.Get(), static_cast<float>(mouse_x),
                              static_cast<float>(mouse_y), &logical_mouse_x,
                              &logical_mouse_y);  // da coords logicas

    float scale_x = static_cast<float>(window.GetWidth()) / CAMERA_WIDTH;
    float scale_y = static_cast<float>(window.GetHeight()) / CAMERA_HEIGHT;

    int scaled_width = static_cast<int>(20 * scale_x);
    int scaled_height = static_cast<int>(20 * scale_y);
    int scale = std::min(scaled_width, scaled_height);

    Crosshairs color = get_crosshair_color(static_cast<int>(logical_mouse_x),
                                           static_cast<int>(logical_mouse_y), snapshot, local_info);

    const BlockTextureInfo& crosshair_info = texture_parser.get_crosshair_texture(color);
    SDL2pp::Texture& crosshair_texture = texture_manager.get_texture(crosshair_info.tileset_path);

    SDL2pp::Rect src(crosshair_info.x, crosshair_info.y, crosshair_info.width,
                     crosshair_info.height);
    SDL2pp::Rect dst(mouse_x - scale / 2, mouse_y - scale / 2, scale, scale);

    renderer.SetLogicalSize(window.GetWidth(), window.GetHeight());
    renderer.Copy(crosshair_texture, src, dst);
    renderer.SetLogicalSize(CAMERA_WIDTH, CAMERA_HEIGHT);
}


void SDLManager::show_screen() { renderer.Present(); }
