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
        mix(MIX_INIT_OGG | MIX_INIT_MP3),
        mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024),  // jugar con valor 1024
        window("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_INITIAL_WIDTH,
               WINDOW_INITIAL_HEIGHT, SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        texture_manager(renderer),
        camera(CAMERA_WIDTH, CAMERA_HEIGHT),
        sounds(mixer, texture_manager, texture_parser),
        animation(renderer, camera, texture_manager, texture_parser),
        shop(renderer, mixer, texture_manager, texture_parser, sounds) {
    renderer.SetLogicalSize(CAMERA_WIDTH, CAMERA_HEIGHT);
    SDL_ShowCursor(SDL_DISABLE);
    mixer.AllocateChannels(30);
}

void SDLManager::set_map(GameMapDTO game_map) { map = std::move(game_map); }

void SDLManager::set_shop(const ShopInfoDTO& shop_info) { shop.set_shop_info(shop_info); }

void SDLManager::set_sound_info(const std::vector<std::string>& usernames) {
    sounds.initialize_channels(usernames);
}

void SDLManager::render_waiting_screen(int players_connected, int players_required,
                                       const std::string& gamename, int iteration, int FPS) {

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
    // una vez que sea cte el player thickness en el server, ponemos su resultado.
    int size_player = PLAYER_THICKNESS / GRAPHIC_SCALE;
    camera.follow(player_x + size_player / 2, player_y + size_player / 2);
}

/* Devuelve toda la informacion para dibujar player y armas en base al arma equipada */
GunVisualData SDLManager::get_gun_visual_info(WeaponType equipped, GunType gun_type) {
    switch (equipped) {
        case KNIFE:
            return GunVisualData{CARRY_KNIFE, KNIFE_GAME, 18, -10, 0, 0, 20, 40, 0};
        case SECONDARY:
            return GunVisualData{CARRY_SECONDARY, GLOCK_GAME, 0, -17, 0, -17, 32, 32, 3};
        case PRIMARY:
            switch (gun_type) {
                case AK47:
                    return GunVisualData{CARRY_PRIMARY, AK47_GAME, 0, -17, 0, -17, 32, 32, 3};
                case AWP:
                    return GunVisualData{CARRY_PRIMARY, AWP_GAME, 0, -17, -2, -30, 32, 32, 8};
                case M3:
                    return GunVisualData{CARRY_PRIMARY, M3_GAME, 0, -17, 0, -17, 32, 32, 5};
                default:
                    // nunca deberia llegar aca
                    return GunVisualData{CARRY_PRIMARY, AK47_GAME, 0, -17, 0, -17, 32, 32, 0};
            }
        default:
            return GunVisualData{CARRY_KNIFE, KNIFE_GAME, 0, 0, 0, 0, 32, 32, 0};
    }
}

/* Renderiza un jugador */
void SDLManager::render_player(const PlayerInfo& p, const CounterTerroristSkin& ct_skin,
                               const TerroristSkin& tt_skin) {
    double angulo = p.orientation + PLAYER_SPRITE_GAP;
    int x_pos = p.x;
    int y_pos = p.y;
    int size_player = PLAYER_THICKNESS / GRAPHIC_SCALE;

    SDL2pp::Rect destino_mundo(x_pos / GRAPHIC_SCALE, y_pos / GRAPHIC_SCALE, size_player,
                               size_player);

    if (!camera.is_visible(destino_mundo))
        return;

    GunVisualData gun_info = get_gun_visual_info(p.equipped, p.primary_gun);
    const BlockTextureInfo& sprite_info =
            p.is_ct ? texture_parser.get_ct_texture(ct_skin, gun_info.carry_sprite) :
                      texture_parser.get_tt_texture(tt_skin, gun_info.carry_sprite);

    SDL2pp::Rect rect_origen(sprite_info.x, sprite_info.y, sprite_info.width, sprite_info.height);

    SDL2pp::Rect destino_camera = camera.rect_world_to_screen(destino_mundo);

    if (p.shoot) {
        double rad = (angulo - 90) * M_PI / 180.0;
        int recoil_x = static_cast<int>(std::cos(rad) * gun_info.recoil);
        int recoil_y = static_cast<int>(std::sin(rad) * gun_info.recoil);
        destino_camera =
                SDL2pp::Rect(destino_camera.GetX() - recoil_x, destino_camera.GetY() - recoil_y,
                             destino_camera.GetW(), destino_camera.GetH());
    }

    std::string path = sprite_info.tileset_path;
    SDL2pp::Texture& skin_texture = texture_manager.get_texture(path);
    renderer.Copy(skin_texture, rect_origen, destino_camera, angulo, SDL2pp::NullOpt);

    SDL2pp::Point centro(destino_camera.GetX() + destino_camera.GetW() / 2,
                         destino_camera.GetY() + destino_camera.GetH() / 2);
    sounds.play_step(p.username, centro, p.movement);
}

// falta hacer que quizas podes no ver el player pero si el arma (x la camera)
// no dibujo las armas junto a cada player para que todas las armas se dibujen sobre los demas
// players (z order)
/* Renderiza las armas de cada jugador */
void SDLManager::render_player_weapon(const PlayerInfo& p) {
    double angulo = p.orientation + PLAYER_SPRITE_GAP;
    int x_pos = p.x;
    int y_pos = p.y;
    int size_player = PLAYER_THICKNESS / GRAPHIC_SCALE;

    SDL2pp::Rect destino_mundo(x_pos / GRAPHIC_SCALE, y_pos / GRAPHIC_SCALE, size_player,
                               size_player);
    SDL2pp::Rect destino_camera = camera.rect_world_to_screen(destino_mundo);  // calcular SIEMPRE

    GunVisualData gun_info = get_gun_visual_info(p.equipped, p.primary_gun);

    // Solo renderizamos el arma si el jugador es visible
    if (camera.is_visible(destino_mundo)) {
        std::string weapon_path = texture_parser.get_gun_texture(gun_info.weapon_sprite);

        int gun_x = destino_camera.GetX() + gun_info.sprite_offset_x;
        int gun_y = destino_camera.GetY() + gun_info.sprite_offset_y;

        if (p.shoot) {
            double rad = (angulo - 90) * M_PI / 180.0;
            gun_x -= static_cast<int>(std::cos(rad) * gun_info.recoil);
            gun_y -= static_cast<int>(std::sin(rad) * gun_info.recoil);
        }

        SDL2pp::Rect gun_dst(gun_x, gun_y, gun_info.width, gun_info.height);

        if (gun_info.weapon_sprite == KNIFE_GAME)
            angulo -= 110;

        SDL2pp::Point rotate(-gun_info.sprite_offset_x + size_player / 2,
                             -gun_info.sprite_offset_y + size_player / 2);

        SDL2pp::Texture& weapon_texture = texture_manager.get_texture(weapon_path);
        renderer.Copy(weapon_texture, SDL2pp::NullOpt, gun_dst, angulo, rotate);
    }

    // Siempre mostramos disparo este o no visible al jugador
    if (p.shoot) {

        int cx = destino_camera.GetX() + destino_camera.GetW() / 2;
        int cy = destino_camera.GetY() + destino_camera.GetH() / 2;

        // Vector desde el centro del jugador hasta la punta del arma (sin rotar)
        SDL2pp::Point local_offset(gun_info.shot_offset_x + gun_info.width / 2 - size_player / 2,
                                   gun_info.shot_offset_y + gun_info.height / 2 - size_player / 2);

        double rad = angulo * M_PI / 180.0;
        double rotated_x =
                local_offset.GetX() * std::cos(rad) - local_offset.GetY() * std::sin(rad);
        double rotated_y =
                local_offset.GetX() * std::sin(rad) + local_offset.GetY() * std::cos(rad);

        SDL2pp::Point origin(static_cast<int>(cx + rotated_x), static_cast<int>(cy + rotated_y));
        if (p.equipped == SECONDARY) {
            animation.render_shot(origin, angulo - 90, p.shot_distance / GRAPHIC_SCALE,
                                  p.secondary_gun);
        } else if (p.equipped != KNIFE && p.equipped != BOMB) {
            animation.render_shot(origin, angulo - 90, p.shot_distance / GRAPHIC_SCALE,
                                  p.primary_gun);
        }
        SDL2pp::Point centro(cx, cy);
        if (p.equipped == SECONDARY) {
            sounds.play_shot(p.username, p.secondary_gun, centro);
        } else if (p.equipped == KNIFE) {
            sounds.play_shot(p.username, NONE, centro);
        } else if (p.equipped == PRIMARY) {
            sounds.play_shot(p.username, p.primary_gun, centro);
        }
    }
}

void SDLManager::render_fov(float orientation) {
    const int diagonal = static_cast<int>(
            std::ceil(std::sqrt(CAMERA_WIDTH * CAMERA_WIDTH + CAMERA_HEIGHT * CAMERA_HEIGHT)));
    float opacity = 0.9f;
    int fov_angle = 90;

    SDL2pp::Texture& fov_texture = texture_manager.get_fov_texture(fov_angle, opacity, diagonal);

    SDL2pp::Rect dest_rect((CAMERA_WIDTH / 2) - (diagonal / 2),
                           (CAMERA_HEIGHT / 2) - (diagonal / 2), diagonal, diagonal);
    renderer.Copy(fov_texture, SDL2pp::NullOpt, dest_rect,
                  orientation - PLAYER_SPRITE_GAP);  // PLAYER_SPRITE_GAP desfasaje textura cono
}


/* Renderiza el tiempo restante de la ronda del HUD */
void SDLManager::render_hud_time(int time_left) {
    // quizas este calculo procesarlo al recibir la snapshot si no necesito el tiempo para otra
    // cosa.
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
void SDLManager::render_hud_life(int life) {

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

void SDLManager::render_in_z_order(const LocalInfo& local_info, int it) {
    update_camera(local_info.player.x / GRAPHIC_SCALE, local_info.player.y / GRAPHIC_SCALE);

    (void)it;

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
                int block_size = BLOCK_THICKNESS / GRAPHIC_SCALE;
                SDL2pp::Rect destino_mundo(vec.x * block_size, vec.y * block_size, block_size,
                                           block_size);
                if (!camera.is_visible(destino_mundo))
                    continue;
                SDL2pp::Rect destino_camera = camera.rect_world_to_screen(destino_mundo);
                renderer.Copy(obj_texture, rect_origen, destino_camera);
            }
        }
    }

    // render de mi player
    render_player(local_info.player, local_info.ct_skin, local_info.tt_skin);
    for (const PlayerInfo& p: local_info.ct_players)
        render_player(p, local_info.ct_skin, local_info.tt_skin);
    for (const PlayerInfo& p: local_info.tt_players)
        render_player(p, local_info.ct_skin, local_info.tt_skin);

    // Renderizo las armas luego de los players para que aparezcan por encima
    render_player_weapon(local_info.player);
    for (const PlayerInfo& p: local_info.ct_players) render_player_weapon(p);
    for (const PlayerInfo& p: local_info.tt_players) render_player_weapon(p);


    render_fov(local_info.player.orientation + PLAYER_SPRITE_GAP);

    render_hud_time(local_info.time_left);
    render_hud_life(local_info.player.life);
    render_hud_ammo(local_info.player.equipped_gun_ammo);
    render_hud_money(local_info.player.money);
}

std::optional<ShopButtonType> SDLManager::interact_button(int x, int y, int money, GunType primary,
                                                          bool click) {
    return shop.interact_button(x, y, money, primary, click);
}


void SDLManager::render_shop(int player_money, GunType primary_gun, GunType secondary_gun) {
    shop.render(player_money, primary_gun, secondary_gun);
}

/* Devuelve el color de la mira a usar dependiendo donde esta posicionado el mouse */
Crosshairs SDLManager::get_crosshair_color(int mouse_x, int mouse_y, const LocalInfo& local_info) {

    const auto& enemies = local_info.player.is_ct ? local_info.tt_players : local_info.ct_players;
    int size_player = PLAYER_THICKNESS / GRAPHIC_SCALE;

    for (const auto& e: enemies) {
        SDL2pp::Rect destino_mundo(e.x / GRAPHIC_SCALE, e.y / GRAPHIC_SCALE, size_player,
                                   size_player);

        if (!camera.is_visible(destino_mundo)) {
            continue;
        }

        SDL2pp::Rect destino_camera = camera.rect_world_to_screen(destino_mundo);

        if (destino_camera.Contains(mouse_x, mouse_y)) {
            return RED;
        }
    }

    return GREEN;
}


void SDLManager::render_crosshair(const LocalInfo& local_info) {

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);  // da coords fisicas

    float logical_mouse_x, logical_mouse_y;
    SDL_RenderWindowToLogical(renderer.Get(), static_cast<float>(mouse_x),
                              static_cast<float>(mouse_y), &logical_mouse_x,
                              &logical_mouse_y);  // da coords logicas

    int size = 20;

    Crosshairs color = get_crosshair_color(static_cast<int>(logical_mouse_x),
                                           static_cast<int>(logical_mouse_y), local_info);

    const BlockTextureInfo& crosshair_info = texture_parser.get_crosshair_texture(color);
    SDL2pp::Texture& crosshair_texture = texture_manager.get_texture(crosshair_info.tileset_path);

    SDL2pp::Rect src(crosshair_info.x, crosshair_info.y, crosshair_info.width,
                     crosshair_info.height);
    SDL2pp::Rect dst(logical_mouse_x - size / 2, logical_mouse_y - size / 2, size, size);

    renderer.Copy(crosshair_texture, src, dst);
    //  el mouse no se ve arriba de los bordes negros (ver si solucionar)
}


void SDLManager::show_screen() { renderer.Present(); }
