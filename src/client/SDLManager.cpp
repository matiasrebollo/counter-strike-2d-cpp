#include "SDLManager.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include "../common/block_texture_parser.h"
#include "../common/settings.h"

SDLManager::SDLManager():
        sdl(SDL_INIT_VIDEO),
        mix(MIX_INIT_OGG | MIX_INIT_MP3),
        mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024),
        window("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
               Settings::getInstance().get_window_initial_width(),
               Settings::getInstance().get_window_initial_height(),
               Settings::getInstance().get_fullscreen() ? SDL_WINDOW_FULLSCREEN_DESKTOP :
                                                          SDL_WINDOW_RESIZABLE),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        texture_manager(renderer),
        camera(CAMERA_WIDTH, CAMERA_HEIGHT),
        sounds(mixer, texture_manager, texture_parser),
        animation(renderer, camera, texture_manager, texture_parser),
        shop(renderer, mixer, texture_manager, texture_parser, sounds) {
    renderer.SetLogicalSize(CAMERA_WIDTH, CAMERA_HEIGHT);
    SDL_SetWindowTitle(window.Get(), Settings::getInstance().get_sdl_window_title().c_str());
    SDL_ShowCursor(SDL_DISABLE);
    mixer.AllocateChannels(30);
}

void SDLManager::set_map(GameMapDTO game_map) { map = std::move(game_map); }

void SDLManager::set_shop(const ShopInfoDTO& shop_info) { shop.set_shop_info(shop_info); }

void SDLManager::set_sound_info(const std::vector<std::string>& usernames) {
    sounds.initialize_channels(usernames);
}

void SDLManager::render_waiting_screen(int players_connected, int players_required, bool is_creator,
                                       const std::string& gamename, int iteration, int FPS,
                                       bool have_ended) {
    int large_font_size = 53;
    int small_font_size = 27;

    const std::string& font_path = texture_parser.get_fw_texture(FONT_WAITING);
    const std::string& background_path = texture_parser.get_fw_texture(BACKGROUND);

    SDL2pp::Texture& background = texture_manager.get_texture(background_path);
    SDL2pp::Rect backgroundRect(0, 0, CAMERA_WIDTH, CAMERA_HEIGHT);
    renderer.Copy(background, SDL2pp::NullOpt, backgroundRect);

    if (have_ended) {
        std::string ended = "Server has been closed!";
        SDL2pp::Texture& have_ended_texture = texture_manager.get_text_texture(
                ended, font_path, small_font_size, SDL2pp::Color(255, 255, 255));
        int ended_width = have_ended_texture.GetWidth();
        int ended_height = have_ended_texture.GetHeight();
        SDL2pp::Rect ended_rect((CAMERA_WIDTH / 2) - (ended_width / 2),
                                (CAMERA_HEIGHT / 2) - (ended_height / 2), ended_width,
                                ended_height);
        renderer.Copy(have_ended_texture, SDL2pp::NullOpt, ended_rect);
        return;
    }

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

    if (is_creator && players_connected >= std::max(players_required / 2, 2)) {
        std::string ended = "Press K to start the game";
        SDL2pp::Texture& have_ended_texture = texture_manager.get_text_texture(
                ended, font_path, small_font_size, SDL2pp::Color(255, 255, 255));
        int ended_width = have_ended_texture.GetWidth();
        int ended_height = have_ended_texture.GetHeight();
        SDL2pp::Rect ended_rect((CAMERA_WIDTH / 2) - ended_width / 2, playersRect.y + mainH + 50,
                                ended_width, ended_height);
        renderer.Copy(have_ended_texture, SDL2pp::NullOpt, ended_rect);
    }

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
    int size_player = PLAYER_THICKNESS / GRAPHIC_SCALE;
    camera.follow(player_x + size_player / 2, player_y + size_player / 2);
}

/* Devuelve toda la informacion para dibujar player y armas en base al arma equipada */
GunVisualData SDLManager::get_gun_visual_info(WeaponType equipped, GunType gun_type) {
    switch (equipped) {
        case KNIFE:
            return GunVisualData{CARRY_KNIFE, KNIFE_GAME, KNIFE_GAME, 18, -10, 0, 0, 20, 40, 0};
        case SECONDARY:
            return GunVisualData{
                    CARRY_SECONDARY, GLOCK_GAME, GLOCK_FLOOR, 0, -17, 0, -17, 32, 32, 3};
        case PRIMARY:
            switch (gun_type) {
                case AK47:
                    return GunVisualData{CARRY_PRIMARY, AK47_GAME, AK47_FLOOR, 0, -17, -1,
                                         -25,           32,        32,         3};
                case AWP:
                    return GunVisualData{CARRY_PRIMARY, AWP_GAME, AWP_FLOOR, 0, -17, -2,
                                         -30,           32,       32,        8};
                case M3:
                    return GunVisualData{CARRY_PRIMARY, M3_GAME, M3_FLOOR, 0, -17, 0,
                                         -17,           32,      32,       5};
                default:
                    // nunca deberia llegar aca
                    return GunVisualData{CARRY_PRIMARY, AK47_GAME, AK47_FLOOR, 0, -17, 0,
                                         -17,           32,        32,         0};
            }
        case BOMB:
            return GunVisualData{CARRY_BOMB, BOMB_GAME, BOMB_GAME, 10, -5, 0, 0, 10, 10, 0};
        default:
            return GunVisualData{CARRY_KNIFE, KNIFE_GAME, KNIFE_GAME, 0, 0, 0, 0, 32, 32, 0};
    }
}

void SDLManager::render_blood_spots(const LocalInfo& local_info) {
    const std::string& path = texture_parser.get_other_path(BLOOD);
    SDL2pp::Texture& blood_texture = texture_manager.get_texture(path);
    blood_texture.SetAlphaMod(200);
    for (const BloodSpot& spot: local_info.blood_spots) {
        SDL2pp::Rect dst_world(spot.x / GRAPHIC_SCALE, spot.y / GRAPHIC_SCALE, spot.size,
                               spot.size);
        if (!camera.is_visible(dst_world))
            continue;
        SDL2pp::Rect dst_camera = camera.rect_world_to_screen(dst_world);
        renderer.Copy(blood_texture, SDL2pp::NullOpt, dst_camera);
    }
}


void SDLManager::render_dead_players(const std::string& username, const PlayerInfo& p) {

    if (p.life != 0)
        return;

    SDL2pp::Rect destino_mundo(p.x / GRAPHIC_SCALE, p.y / GRAPHIC_SCALE,
                               PLAYER_THICKNESS / GRAPHIC_SCALE, PLAYER_THICKNESS / GRAPHIC_SCALE);

    if (!camera.is_visible(destino_mundo))
        return;

    SDL2pp::Rect destino_camera = camera.rect_world_to_screen(destino_mundo);
    SDL2pp::Rect sangre_rect(destino_camera.GetX() - 5, destino_camera.GetY() - 5,
                             destino_camera.GetW() + 10, destino_camera.GetH() + 10);

    // Render BLOOD texture
    const std::string& path = texture_parser.get_other_path(BLOOD);
    SDL2pp::Texture& blood_texture = texture_manager.get_texture(path);
    blood_texture.SetAlphaMod(200);
    renderer.Copy(blood_texture, SDL2pp::NullOpt, sangre_rect);

    if (p.just_died) {
        SDL2pp::Point centro(destino_camera.GetX() + destino_camera.GetW() / 2,
                             destino_camera.GetY() + destino_camera.GetH() / 2);
        sounds.play_death(username, centro);
    }
}

void SDLManager::render_dropped_items(const std::vector<Drop>& drops) {
    for (const auto& drop: drops) {
        SDL2pp::Rect destino_mundo(drop.position.x / GRAPHIC_SCALE, drop.position.y / GRAPHIC_SCALE,
                                   ITEM_THICKNESS / GRAPHIC_SCALE, ITEM_THICKNESS / GRAPHIC_SCALE);

        if (!camera.is_visible(destino_mundo))
            continue;

        SDL2pp::Rect destino_camera = camera.rect_world_to_screen(destino_mundo);

        std::string path;
        if (drop.is_bomb) {
            path = texture_parser.get_gun_texture(BOMB_GAME);

            destino_camera = SDL2pp::Rect(destino_camera.GetX(), destino_camera.GetY(),
                                          destino_camera.GetW() / 2, destino_camera.GetH() / 2);
        } else {
            std::string shadow_path = texture_parser.get_other_path(ITEM_SHADOW);
            SDL2pp::Texture& shadow_texture = texture_manager.get_texture(shadow_path);
            shadow_texture.SetBlendMode(SDL_BLENDMODE_ADD);
            renderer.Copy(shadow_texture, SDL2pp::NullOpt, destino_camera);

            GunSprites sprite = get_gun_visual_info(PRIMARY, drop.gun_type).floor_sprite;
            path = texture_parser.get_gun_texture(sprite);
        }

        SDL2pp::Texture& texture = texture_manager.get_texture(path);
        renderer.Copy(texture, SDL2pp::NullOpt, destino_camera);
    }
}


/* Renderiza la bomba en el suelo si esta plantada */
void SDLManager::render_bomb(const LocalInfo& local_info, int it) {
    if (local_info.bomb_status == BombStatus::NOT_PLANTED)
        return;

    if (local_info.just_planted)
        sounds.play_bomb_sound(BOMB_PLANTED);
    if (local_info.just_defused)
        sounds.play_bomb_sound(BOMB_DEFUSE);

    SDL2pp::Rect destino_mundo(local_info.bomb_planted_x / GRAPHIC_SCALE,
                               local_info.bomb_planted_y / GRAPHIC_SCALE,
                               BOMB_THICKNESS / GRAPHIC_SCALE, BOMB_THICKNESS / GRAPHIC_SCALE);
    SDL2pp::Rect destino_camera = camera.rect_world_to_screen(destino_mundo);
    SDL2pp::Point centro(destino_camera.GetX() + destino_camera.GetW() / 2,
                         destino_camera.GetY() + destino_camera.GetH() / 2);

    SDL2pp::Point world_position(local_info.bomb_planted_x, local_info.bomb_planted_y);

    if (local_info.exploded) {
        animation.start_bomb_explosion(it, world_position);
        animation.start_camera_shake(it);
        sounds.play_bomb_explosion(centro);
    }

    if (animation.is_bomb_explosion_active(it)) {
        animation.render_bomb_explosion(it);
        return;
    }

    if (!camera.is_visible(destino_mundo) || local_info.bomb_status == BombStatus::EXPLODED)
        return;

    std::string path = texture_parser.get_gun_texture(BOMB_GAME);
    SDL2pp::Texture& bomb_texture = texture_manager.get_texture(path);
    if (local_info.bomb_status == BombStatus::PLANTED) {
        sounds.play_bomb_tick(centro);
    }
    renderer.Copy(bomb_texture, SDL2pp::NullOpt, destino_camera);
}

/* Renderiza un jugador */
void SDLManager::render_player(const std::string& username, const PlayerInfo& p,
                               const CounterTerroristSkin& ct_skin, const TerroristSkin& tt_skin,
                               int it) {
    if (p.life == 0)
        return;

    double angulo = p.orientation + PLAYER_SPRITE_GAP;
    int x_pos = p.x;
    int y_pos = p.y;
    int size_player = PLAYER_THICKNESS / GRAPHIC_SCALE;

    SDL2pp::Rect destino_mundo(x_pos / GRAPHIC_SCALE, y_pos / GRAPHIC_SCALE, size_player,
                               size_player);

    if (!camera.is_visible(destino_mundo))
        return;

    GunVisualData gun_player_info = get_gun_visual_info(p.equipped, p.primary_gun);
    const BlockTextureInfo& sprite_info =
            p.is_ct ? texture_parser.get_ct_texture(ct_skin, gun_player_info.carry_sprite) :
                      texture_parser.get_tt_texture(tt_skin, gun_player_info.carry_sprite);

    SDL2pp::Rect rect_origen(sprite_info.x, sprite_info.y, sprite_info.width, sprite_info.height);

    SDL2pp::Rect destino_camera = camera.rect_world_to_screen(destino_mundo);

    bool apply_recoil = p.shoot || animation.is_shot_active(username, it);

    if (apply_recoil) {
        double rad = (angulo - 90) * M_PI / 180.0;
        int recoil_x = static_cast<int>(std::cos(rad) * gun_player_info.recoil);
        int recoil_y = static_cast<int>(std::sin(rad) * gun_player_info.recoil);
        destino_camera =
                SDL2pp::Rect(destino_camera.GetX() - recoil_x, destino_camera.GetY() - recoil_y,
                             destino_camera.GetW(), destino_camera.GetH());
    }

    std::string path = sprite_info.tileset_path;
    SDL2pp::Texture& skin_texture = texture_manager.get_texture(path);
    renderer.Copy(skin_texture, rect_origen, destino_camera, angulo, SDL2pp::NullOpt);

    SDL2pp::Point centro(destino_camera.GetX() + destino_camera.GetW() / 2,
                         destino_camera.GetY() + destino_camera.GetH() / 2);

    sounds.play_step(username, centro, p.movement);
    sounds.play_bomb_action(centro, p.is_planting, p.is_defusing);
}

void SDLManager::render_player_shot(const GunVisualData& gun_info, const std::string& username,
                                    const PlayerInfo& p, const SDL2pp::Rect& destino_camera,
                                    int it) {

    SDL2pp::Point end_world(p.impact_position_x / GRAPHIC_SCALE,
                            p.impact_position_y / GRAPHIC_SCALE);

    if (p.shoot && p.equipped != KNIFE && p.equipped != BOMB) {
        int duration = 1;  // por defecto

        if (p.equipped == PRIMARY) {
            if (p.primary_gun == AK47)
                duration = 5;
            else
                duration = 2;
        } else if (p.equipped == SECONDARY) {
            duration = 2;
        }

        animation.start_shot(username, it, end_world, duration);
    }

    if (animation.is_shot_active(username, it)) {
        int size_player = PLAYER_THICKNESS / GRAPHIC_SCALE;
        int cx = destino_camera.GetX() + destino_camera.GetW() / 2;
        int cy = destino_camera.GetY() + destino_camera.GetH() / 2;

        SDL2pp::Point local_offset(gun_info.shot_offset_x + gun_info.width / 2 - size_player / 2,
                                   gun_info.shot_offset_y + gun_info.height / 2 - size_player / 2);

        double rad = (p.orientation + PLAYER_SPRITE_GAP) * M_PI / 180.0;
        double rotated_x =
                local_offset.GetX() * std::cos(rad) - local_offset.GetY() * std::sin(rad);
        double rotated_y =
                local_offset.GetX() * std::sin(rad) + local_offset.GetY() * std::cos(rad);

        SDL2pp::Point origin(static_cast<int>(cx + rotated_x), static_cast<int>(cy + rotated_y));

        if (p.equipped == SECONDARY) {
            animation.render_shot(origin, animation.get_shot_impact(username), p.secondary_gun,
                                  p.orientation);
        } else if (p.equipped != KNIFE && p.equipped != BOMB) {
            animation.render_shot(origin, animation.get_shot_impact(username), p.primary_gun,
                                  p.orientation);
        }
    }


    bool play_sound = false;

    if (p.primary_gun == AK47 && p.equipped == PRIMARY) {
        play_sound = animation.is_shot_active(username, it);
    } else {
        play_sound = p.shoot;
    }
    if (play_sound) {
        SDL2pp::Point centro(destino_camera.GetX() + destino_camera.GetW() / 2,
                             destino_camera.GetY() + destino_camera.GetH() / 2);

        if (p.equipped == SECONDARY) {
            sounds.play_shot(username, p.secondary_gun, centro);
        } else if (p.equipped == KNIFE) {
            sounds.play_shot(username, NONE, centro);
        } else if (p.equipped == PRIMARY) {
            sounds.play_shot(username, p.primary_gun, centro);
        }
    }
}

/* Renderiza las armas de cada jugador */
void SDLManager::render_player_weapon(const std::string& username, const PlayerInfo& p, int it) {
    if (p.life == 0)
        return;

    double angulo = p.orientation + PLAYER_SPRITE_GAP;
    int x_pos = p.x;
    int y_pos = p.y;
    int size_player = PLAYER_THICKNESS / GRAPHIC_SCALE;

    SDL2pp::Rect destino_mundo(x_pos / GRAPHIC_SCALE, y_pos / GRAPHIC_SCALE, size_player,
                               size_player);
    SDL2pp::Rect destino_camera = camera.rect_world_to_screen(destino_mundo);

    GunVisualData gun_info = get_gun_visual_info(p.equipped, p.primary_gun);

    if (camera.is_visible(destino_mundo)) {
        std::string weapon_path = texture_parser.get_gun_texture(gun_info.weapon_sprite);

        int gun_x = destino_camera.GetX() + gun_info.sprite_offset_x;
        int gun_y = destino_camera.GetY() + gun_info.sprite_offset_y;

        bool apply_recoil = p.shoot || animation.is_shot_active(username, it);

        if (apply_recoil) {
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
    render_player_shot(gun_info, username, p, destino_camera, it);
}

void SDLManager::render_fov(float orientation) {
    const int diagonal = static_cast<int>(
            std::ceil(std::sqrt(CAMERA_WIDTH * CAMERA_WIDTH + CAMERA_HEIGHT * CAMERA_HEIGHT)));
    float opacity = 0.9f;
    int fov_angle = Settings::getInstance().get_fov_angle();

    SDL2pp::Texture& fov_texture = texture_manager.get_fov_texture(fov_angle, opacity, diagonal);

    SDL2pp::Rect dest_rect((CAMERA_WIDTH / 2) - (diagonal / 2),
                           (CAMERA_HEIGHT / 2) - (diagonal / 2), diagonal, diagonal);
    renderer.Copy(fov_texture, SDL2pp::NullOpt, dest_rect, orientation - PLAYER_SPRITE_GAP);
}


void SDLManager::render_if_dead_or_damaged(const int& life, bool received_damage, int it) {
    if (life == 0) {
        renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        SDL2pp::Rect dst(0, 0, CAMERA_WIDTH, CAMERA_HEIGHT);

        const std::string& path = texture_parser.get_other_path(BLOOD_SCREEN);
        SDL2pp::Texture& blood_texture = texture_manager.get_texture(path);
        blood_texture.SetAlphaMod(100);
        renderer.Copy(blood_texture, SDL2pp::NullOpt, dst);

        renderer.SetDrawColor(255, 0, 0, 40);
        renderer.FillRect(dst);

        int font_size = 20;
        const std::string& font_path = texture_parser.get_fw_texture(FONT_WAITING);
        std::string message_dead = "You are dead!";
        SDL2pp::Texture& round_texture = texture_manager.get_text_texture(
                message_dead, font_path, font_size, SDL2pp::Color(255, 255, 0));
        round_texture.SetAlphaMod(190);
        SDL2pp::Rect dstRect((CAMERA_WIDTH - round_texture.GetWidth()) / 2, 100,
                             round_texture.GetWidth(), round_texture.GetHeight());
        renderer.Copy(round_texture, SDL2pp::NullOpt, dstRect);
        renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);
        renderer.SetDrawColor(0, 0, 0, 255);
        return;
    }

    if (received_damage) {
        animation.start_damage_overlay(it);
    }
    if (animation.is_damage_overlay_active(it)) {
        animation.render_damage_overlay(it);
        return;
    }
}


void SDLManager::render_hud_bomb_not_planted_time(int minutes, int seconds, Phase phase) {
    std::stringstream ss;
    ss << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;
    std::string time_str = ss.str();

    int clock_width = 30, clock_height = 33;
    int char_width = 24;
    int char_height = 33;
    int dp_width = 5;
    int spacing = 2;

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
    int r, g, b;

    if (phase == Phase::ATTACK && minutes == 0 && seconds <= 10) {
        r = 255;
        g = 0;
        b = 0;
    } else {
        r = 255;
        g = 255;
        b = 0;
    }
    clock_texture.SetColorMod(r, g, b);
    clock_texture.SetAlphaMod(190);
    SDL2pp::Rect clock_src(clock_info.x, clock_info.y, clock_info.width, clock_info.height);
    SDL2pp::Rect clock_dst(start_x, y, clock_width, clock_height);
    renderer.Copy(clock_texture, clock_src, clock_dst);

    int x = start_x + clock_width + spacing;
    for (char c: time_str) {
        HudNumbers num_enum = (c == ':') ? DP : static_cast<HudNumbers>(c - '0');
        const BlockTextureInfo& sprite_info = texture_parser.get_number_texture(num_enum);
        SDL2pp::Texture& texture = texture_manager.get_texture(sprite_info.tileset_path);
        texture.SetColorMod(r, g, b);
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

void SDLManager::render_hud_bomb_explotion_time(int minutes, int seconds) {
    std::stringstream ss;
    ss << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;
    std::string time_str = ss.str();

    int bomb_width = 30, bomb_height = 33;
    int char_width = 24;
    int char_height = 33;
    int dp_width = 5;
    int spacing = 2;

    int text_width = std::accumulate(time_str.begin(), time_str.end(), 0,
                                     [char_width, dp_width, spacing](int sum, char c) {
                                         return sum + (c == ':' ? dp_width : char_width) + spacing;
                                     }) -
                     spacing;

    int text_x = (CAMERA_WIDTH - text_width) / 2;
    int bomb_x = text_x - bomb_width - spacing;
    int y = CAMERA_HEIGHT - char_height;

    const BlockTextureInfo& bomb_info = texture_parser.get_symbol_texture(BOMB_ACTIVE);
    SDL2pp::Texture& bomb_texture = texture_manager.get_texture(bomb_info.tileset_path);

    int r = 255;
    int g = (seconds % 2 == 0) ? 0 : 255;
    int b = 0;

    bomb_texture.SetColorMod(r, g, b);
    bomb_texture.SetAlphaMod(190);

    SDL2pp::Rect clock_src(bomb_info.x, bomb_info.y, bomb_info.width, bomb_info.height);
    SDL2pp::Rect clock_dst(bomb_x, y, bomb_width, bomb_height);
    renderer.Copy(bomb_texture, clock_src, clock_dst);

    int x = text_x;
    for (char c: time_str) {
        HudNumbers num_enum = (c == ':') ? DP : static_cast<HudNumbers>(c - '0');
        const BlockTextureInfo& sprite_info = texture_parser.get_number_texture(num_enum);
        SDL2pp::Texture& texture = texture_manager.get_texture(sprite_info.tileset_path);
        texture.SetColorMod(r, g, b);
        texture.SetAlphaMod(190);

        int width = (c == ':') ? dp_width : char_width;

        SDL2pp::Rect src(sprite_info.x, sprite_info.y, sprite_info.width, sprite_info.height);
        SDL2pp::Rect dst(x, y, width, char_height);
        renderer.Copy(texture, src, dst);

        x += width + spacing;
    }
}

/* Renderiza el tiempo restante de la ronda del HUD */
void SDLManager::render_hud_time(const LocalInfo& local_info) {
    if (local_info.phase == ROUND_ENDED) {
        sounds.stop_clock_sound();
        return;
    }

    int minutes = local_info.time_left / 60;
    int seconds = local_info.time_left % 60;

    if (local_info.phase == ATTACK && local_info.time_left <= 10) {
        sounds.play_clock_sound(FAST_TICK_CLOCK);
    }

    if (local_info.just_planted) {
        sounds.stop_clock_sound();
    }

    if (local_info.bomb_status == BombStatus::NOT_PLANTED) {
        render_hud_bomb_not_planted_time(minutes, seconds, local_info.phase);
    } else if (local_info.bomb_status == BombStatus::PLANTED) {
        render_hud_bomb_explotion_time(minutes, seconds);
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

/* Renderiza la ronda actual */
void SDLManager::render_hud_rounds(size_t ct_wins, size_t tt_wins) {
    int font_size = 15;
    const std::string& font_path = texture_parser.get_fw_texture(FONT_WAITING);

    std::string ct_text = std::to_string(ct_wins);
    std::string vs_text = " vs ";
    std::string tt_text = std::to_string(tt_wins);

    SDL2pp::Texture& ct_texture = texture_manager.get_text_texture(ct_text, font_path, font_size,
                                                                   SDL2pp::Color(33, 97, 140));
    SDL2pp::Texture& vs_texture = texture_manager.get_text_texture(vs_text, font_path, font_size,
                                                                   SDL2pp::Color(255, 255, 255));
    SDL2pp::Texture& tt_texture = texture_manager.get_text_texture(tt_text, font_path, font_size,
                                                                   SDL2pp::Color(183, 149, 11));

    int total_width = ct_texture.GetWidth() + vs_texture.GetWidth() + tt_texture.GetWidth();
    int start_x = (CAMERA_WIDTH - total_width) / 2;
    int y = 10;

    renderer.Copy(ct_texture, SDL2pp::NullOpt,
                  SDL2pp::Rect(start_x, y, ct_texture.GetWidth(), ct_texture.GetHeight()));
    renderer.Copy(vs_texture, SDL2pp::NullOpt,
                  SDL2pp::Rect(start_x + ct_texture.GetWidth(), y, vs_texture.GetWidth(),
                               vs_texture.GetHeight()));
    renderer.Copy(tt_texture, SDL2pp::NullOpt,
                  SDL2pp::Rect(start_x + ct_texture.GetWidth() + vs_texture.GetWidth(), y,
                               tt_texture.GetWidth(), tt_texture.GetHeight()));
}

/* Si hay un ganador en la ronda, se está en unos segundos donde se muestra el ganador, y este
   método lo renderiza */
void SDLManager::render_current_round_winner(const std::optional<Team>& winner,
                                             const Phase& phase) {
    if (phase != ROUND_ENDED) {
        return;
    }
    int font_size = 25;
    const std::string& font_path = texture_parser.get_fw_texture(FONT_WAITING);
    std::string winner_string =
            winner.value() == CT ? "Counter Terrorists win!" : "Terrorists win!";
    SDL2pp::Color color =
            winner.value() == CT ? SDL2pp::Color(33, 97, 140) : SDL2pp::Color(183, 149, 11);

    SDL2pp::Texture& round_texture =
            texture_manager.get_text_texture(winner_string, font_path, font_size, color);
    round_texture.SetAlphaMod(190);


    SDL2pp::Rect dstRect((CAMERA_WIDTH - round_texture.GetWidth()) / 2, 35,
                         round_texture.GetWidth(), round_texture.GetHeight());

    renderer.Copy(round_texture, SDL2pp::NullOpt, dstRect);
}

void SDLManager::render_hud_bomb(const bool& has_bomb, const bool& in_site, const int& seconds) {
    if (!has_bomb) {
        return;
    }
    int plus_width = 30;
    int plus_height = 33;
    int char_height = 33;

    int x = 140;
    int y = CAMERA_HEIGHT - char_height;


    const BlockTextureInfo& bomb_info = texture_parser.get_symbol_texture(BOMB_ACTIVE);
    SDL2pp::Texture& bomb_texture = texture_manager.get_texture(bomb_info.tileset_path);

    int r, g, b;
    if (in_site && seconds % 2 == 0) {
        r = 255;
        g = 0;
        b = 0;
    } else {
        r = 255;
        g = 255;
        b = 0;
    }

    bomb_texture.SetColorMod(r, g, b);
    bomb_texture.SetAlphaMod(200);

    SDL2pp::Rect src_bomb(bomb_info.x, bomb_info.y, bomb_info.width, bomb_info.height);
    SDL2pp::Rect dst_bomb(x, y, plus_width, plus_height);

    renderer.Copy(bomb_texture, src_bomb, dst_bomb);
}

std::pair<std::vector<std::pair<std::string, PlayerInfo>>,
          std::vector<std::pair<std::string, PlayerInfo>>>
        SDLManager::get_teams(const LocalInfo& local_info) {
    std::vector<std::pair<std::string, PlayerInfo>> cts = {};
    std::vector<std::pair<std::string, PlayerInfo>> tts = {};

    for (const auto& [username, p]: local_info.players) {
        if (p.is_ct) {
            cts.emplace_back(username, p);
        } else {
            tts.emplace_back(username, p);
        }
    }
    if (local_info.player.is_ct) {
        cts.emplace_back(local_info.username, local_info.player);
    } else {
        tts.emplace_back(local_info.username, local_info.player);
    }

    auto f_cmp = [](const auto& pj1, const auto& pj2) {
        if (pj1.second.kills != pj2.second.kills)
            return pj1.second.kills > pj2.second.kills;
        return pj1.first < pj2.first;
    };

    std::sort(cts.begin(), cts.end(), f_cmp);
    std::sort(tts.begin(), tts.end(), f_cmp);

    return std::make_pair(cts, tts);
}

void SDLManager::draw_line(const std::string& line, int y, SDL2pp::Color color,
                           const int& size_box) {
    int font_size = static_cast<int>(size_box * 0.15);
    int text_x = size_box + 10;
    const std::string& font_path = texture_parser.get_fw_texture(FONT_WAITING);
    SDL2pp::Texture& text_texture =
            texture_manager.get_text_texture(line, font_path, font_size, color);
    renderer.Copy(text_texture, SDL2pp::NullOpt,
                  SDL2pp::Rect(text_x, y, text_texture.GetWidth(), text_texture.GetHeight()));
}

void SDLManager::draw_player_line(const std::string& username, const PlayerInfo& p,
                                  SDL2pp::Color color, const int& size_box, const int& line_y) {
    std::string prefix = "* " + username + " | Kills: " + std::to_string(p.kills) +
                         " | Deaths: " + std::to_string(p.deaths) + " | Bonifications: ";
    std::string bonus = "$" + std::to_string(std::abs(p.bonifications));
    SDL2pp::Color bonus_color = p.bonifications < 0 ? SDL2pp::Color(231, 76, 60, 255) : color;


    int font_size = static_cast<int>(size_box * 0.15);
    int text_x = size_box + 10;

    const std::string& font_path = texture_parser.get_fw_texture(FONT_WAITING);
    SDL2pp::Texture& prefix_texture =
            texture_manager.get_text_texture(prefix, font_path, font_size, color);
    renderer.Copy(
            prefix_texture, SDL2pp::NullOpt,
            SDL2pp::Rect(text_x, line_y, prefix_texture.GetWidth(), prefix_texture.GetHeight()));

    SDL2pp::Texture& bonus_texture =
            texture_manager.get_text_texture(bonus, font_path, font_size, bonus_color);
    renderer.Copy(bonus_texture, SDL2pp::NullOpt,
                  SDL2pp::Rect(text_x + prefix_texture.GetWidth(), line_y, bonus_texture.GetWidth(),
                               bonus_texture.GetHeight()));
}

void SDLManager::stats_team(int& line_y, const bool& are_ct,
                            const std::vector<std::pair<std::string, PlayerInfo>>& team,
                            const int& size_box, const std::string& username_client) {
    std::string team_s = are_ct ? "Counter Terrorirsts" : "Terrorists";
    SDL2pp::Color blue = {0, 150, 255, 255};
    SDL2pp::Color yellow = {255, 200, 0, 255};
    SDL2pp::Color team_c = are_ct ? blue : yellow;
    SDL2pp::Color white = {255, 255, 255, 255};
    SDL2pp::Color user_color = {68, 179, 157, 255};

    draw_line(team_s, line_y, team_c, size_box);
    int spacing = 15;
    line_y += spacing + 10;

    for (const auto& [username, p]: team) {
        SDL_Color actual_color = username == username_client ? user_color : white;
        draw_player_line(username, p, actual_color, size_box, line_y);
        line_y += spacing;
    }
}

void SDLManager::render_server_closed() {
    int box_width = static_cast<int>(CAMERA_WIDTH * 0.59);
    int box_height = static_cast<int>(CAMERA_HEIGHT * 0.1);
    int box_x = (CAMERA_WIDTH - box_width) / 2;
    int box_y = (CAMERA_HEIGHT - box_height) / 2;

    SDL2pp::Rect stats_box{box_x, box_y, box_width, box_height};
    renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    renderer.SetDrawColor(0, 0, 0, 180);
    renderer.FillRect(stats_box);
    renderer.SetDrawColor(255, 255, 255, 255);
    renderer.DrawRect(stats_box);

    int start_y = stats_box.y + 10;
    SDL2pp::Color red = {255, 0, 0, 255};
    int line_y = start_y;
    this->draw_line("Connection lost with server!", line_y, red, box_x);
}

void SDLManager::render_stats(const LocalInfo& local_info) {
    int box_width = static_cast<int>(CAMERA_WIDTH * 0.7);
    int box_height = static_cast<int>(CAMERA_HEIGHT * 0.8);
    int box_x = (CAMERA_WIDTH - box_width) / 2;
    int box_y = (CAMERA_HEIGHT - box_height) / 2;

    SDL2pp::Rect stats_box{box_x, box_y, box_width, box_height};

    renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    renderer.SetDrawColor(0, 0, 0, 180);
    renderer.FillRect(stats_box);

    renderer.SetDrawColor(255, 255, 255, 255);
    renderer.DrawRect(stats_box);

    std::string result_text;
    SDL2pp::Color result_color;

    bool player_is_ct = local_info.player.is_ct;

    if (local_info.ct_wins == local_info.tt_wins) {
        result_text = "Draw";
        result_color = {200, 200, 200, 255};
    } else {
        bool ct_won = local_info.ct_wins > local_info.tt_wins;
        bool player_won = (player_is_ct && ct_won) || (!player_is_ct && !ct_won);

        if (player_won) {
            result_text = "Victory";
            result_color = {35, 155, 86, 255};
        } else {
            result_text = "Defeat";
            result_color = {231, 76, 60, 255};
        }
    }

    int result_y = stats_box.y + 10;
    this->draw_line(result_text, result_y, result_color, box_x + box_width / 3);

    int start_y = result_y + 30;
    int line_y = start_y;

    auto [cts, tts] = this->get_teams(local_info);

    this->stats_team(line_y, true, cts, box_x, local_info.username);

    line_y = stats_box.y + static_cast<int>(stats_box.h / 1.8);

    this->stats_team(line_y, false, tts, box_x, local_info.username);

    renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);
    renderer.SetDrawColor(0, 0, 0, 255);
}


void SDLManager::render_in_z_order(const LocalInfo& local_info, int it) {
    SDL2pp::Point shake_offset = animation.get_camera_shake(it);
    update_camera((local_info.player.x - shake_offset.GetX()) / GRAPHIC_SCALE,
                  (local_info.player.y - shake_offset.GetY()) / GRAPHIC_SCALE);

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
                if (game_map.sites.find(vec) != game_map.sites.end()) {
                    renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
                    renderer.SetDrawColor(255, 0, 0, 40);
                    renderer.FillRect(destino_camera);
                    renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);
                    renderer.SetDrawColor(0, 0, 0, 255);
                }
            }
        }
    }

    render_blood_spots(local_info);

    render_dead_players(local_info.username, local_info.player);
    for (auto& [username, p]: local_info.players) {
        render_dead_players(username, p);
    }

    render_dropped_items(local_info.drops);

    render_bomb(local_info, it);

    render_player(local_info.username, local_info.player, local_info.ct_skin, local_info.tt_skin,
                  it);
    for (auto& [username, p]: local_info.players) {
        render_player(username, p, local_info.ct_skin, local_info.tt_skin, it);
    }
    render_player_weapon(local_info.username, local_info.player, it);
    for (auto& [username, p]: local_info.players) {
        render_player_weapon(username, p, it);
    }

    if (local_info.server_has_been_closed) {
        render_server_closed();
    } else if (game_ended(local_info)) {
        render_hud_rounds(local_info.ct_wins, local_info.tt_wins);
        render_stats(local_info);
    } else {
        render_fov(local_info.player.orientation + PLAYER_SPRITE_GAP);
        render_if_dead_or_damaged(local_info.player.life, local_info.player.just_damaged, it);
        render_hud_time(local_info);
        render_hud_life(local_info.player.life);
        render_hud_bomb(local_info.player.has_bomb, local_info.player.in_site,
                        local_info.time_left % 60);
        render_hud_rounds(local_info.ct_wins, local_info.tt_wins);
        render_hud_ammo(local_info.player.equipped_gun_ammo);
        render_hud_money(local_info.player.money);
        render_current_round_winner(local_info.current_round_winner, local_info.phase);
    }
}

std::optional<ShopButtonType> SDLManager::interact_button(int x, int y, int money, GunType primary,
                                                          bool click) {
    return shop.interact_button(x, y, money, primary, click);
}


void SDLManager::render_shop(int player_money, GunType primary_gun, GunType secondary_gun) {
    shop.render(player_money, primary_gun, secondary_gun);
}

std::string SDLManager::get_gun_name(GunType gun_type) {
    switch (gun_type) {
        case AK47:
            return "AK47";
        case AWP:
            return "AWP";
        case M3:
            return "M3";
        case GLOCK:
            return "GLOCK";
        case NONE:
            return "NONE";
    }
    return " ";
}

/* Devuelve el color de la mira a usar dependiendo donde esta posicionado el mouse */
Crosshairs SDLManager::get_crosshair_and_render_info(int mouse_x, int mouse_y,
                                                     const LocalInfo& local_info) {
    int size_player = PLAYER_THICKNESS / GRAPHIC_SCALE;

    for (const auto& [_, e]: local_info.players) {
        if ((local_info.player.is_ct && e.is_ct) || (!local_info.player.is_ct && !e.is_ct)) {
            continue;
        }
        if (e.life == 0)
            continue;

        SDL2pp::Rect destino_mundo(e.x / GRAPHIC_SCALE, e.y / GRAPHIC_SCALE, size_player,
                                   size_player);

        if (!camera.is_visible(destino_mundo))
            continue;

        SDL2pp::Rect destino_camera = camera.rect_world_to_screen(destino_mundo);

        if (destino_camera.Contains(mouse_x, mouse_y)) {
            return RED;
        }
    }

    int size_drop = ITEM_THICKNESS / GRAPHIC_SCALE;
    for (const auto& drop: local_info.drops) {
        SDL2pp::Rect destino_mundo(drop.position.x / GRAPHIC_SCALE, drop.position.y / GRAPHIC_SCALE,
                                   size_drop, size_drop);

        if (!camera.is_visible(destino_mundo))
            continue;

        SDL2pp::Rect destino_camera = camera.rect_world_to_screen(destino_mundo);

        if (drop.is_bomb) {
            destino_camera = SDL2pp::Rect(destino_camera.GetX(), destino_camera.GetY(),
                                          destino_camera.GetW() / 2, destino_camera.GetH() / 2);
        }

        if (destino_camera.Contains(mouse_x, mouse_y)) {
            if (!drop.is_bomb) {
                std::string gun_name = get_gun_name(drop.gun_type);
                std::string message = gun_name + " | " + std::to_string(drop.ammo);

                const std::string& font_path = texture_parser.get_fw_texture(FONT_SHOP);
                SDL2pp::Texture& text_texture = texture_manager.get_text_texture(
                        message, font_path, 10, SDL2pp::Color(255, 255, 0));

                int box_width = 50;
                int box_height = 12;
                int box_x = mouse_x + 10;
                int box_y = mouse_y + 10;

                SDL2pp::Rect box_rect(box_x, box_y, box_width, box_height);
                renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
                renderer.SetDrawColor(0, 0, 0, 150);
                renderer.FillRect(box_rect);

                int text_x = box_x + (box_width - text_texture.GetWidth()) / 2;
                int text_y = box_y + (box_height - text_texture.GetHeight()) / 2;
                SDL2pp::Rect dst(text_x, text_y, text_texture.GetWidth(), text_texture.GetHeight());

                renderer.Copy(text_texture, SDL2pp::NullOpt, dst);
                renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);
                renderer.SetDrawColor(0, 0, 0, 255);
            }
            return YELLOW;
        }
    }

    return GREEN;
}


void SDLManager::render_crosshair(const LocalInfo& local_info) {

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);  // coordenadass fisicas

    float logical_mouse_x, logical_mouse_y;
    SDL_RenderWindowToLogical(renderer.Get(), static_cast<float>(mouse_x),
                              static_cast<float>(mouse_y), &logical_mouse_x,
                              &logical_mouse_y);  // coordenadass logicas

    int size = 20;

    Crosshairs color = GREEN;
    if (!shop.is_shop_open())
        color = get_crosshair_and_render_info(static_cast<int>(logical_mouse_x),
                                              static_cast<int>(logical_mouse_y), local_info);

    const BlockTextureInfo& crosshair_info = texture_parser.get_crosshair_texture(color);
    SDL2pp::Texture& crosshair_texture = texture_manager.get_texture(crosshair_info.tileset_path);

    SDL2pp::Rect src(crosshair_info.x, crosshair_info.y, crosshair_info.width,
                     crosshair_info.height);
    SDL2pp::Rect dst(logical_mouse_x - size / 2, logical_mouse_y - size / 2, size, size);

    renderer.Copy(crosshair_texture, src, dst);
}


void SDLManager::show_screen() { renderer.Present(); }

void SDLManager::close_shop() { shop.close_shop(); }

void SDLManager::open_shop() { shop.open_shop(); }

bool SDLManager::game_ended(const LocalInfo& local_info) {
    return local_info.current_round == local_info.total_rounds &&
           local_info.phase == Phase::ROUND_ENDED && local_info.time_left == 0;
}

void SDLManager::make_round_start_sound(const bool& is_ct) {
    if (is_ct) {
        sounds.play_round_sound(SoundEffect::START_ROUND_CT);
    } else {
        sounds.play_round_sound(SoundEffect::START_ROUND_TT);
    }
}

void SDLManager::make_team_winner_sound(std::optional<Team> current_winner) {
    if (!current_winner.has_value()) {
        return;
    } else {
        Team winner = current_winner.value();
        if (winner == Team::CT) {
            sounds.play_round_sound(CT_WINS);
        } else {
            sounds.play_round_sound(TT_WINS);
        }
    }
}

void SDLManager::restart_animations() { this->animation.restart_animations(); }
