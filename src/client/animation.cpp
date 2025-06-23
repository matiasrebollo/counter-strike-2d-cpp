#include "animation.h"

#include <cmath>
#include <string>

Animation::Animation(SDL2pp::Renderer& renderer, Camera& camera, TextureManager& texture_manager,
                     BlockTextureParser& texture_parser):
        renderer(renderer),
        camera(camera),
        texture_manager(texture_manager),
        texture_parser(texture_parser) {}

void Animation::start_shot(const std::string& username, int current_it, SDL2pp::Point impact,
                           int duration) {
    shots[username] = {current_it, impact, duration};
}

bool Animation::is_shot_active(const std::string& username, int current_it) {
    auto it = shots.find(username);
    if (it == shots.end())
        return false;

    int diff = current_it - it->second.start_it;

    // ARREGLAR BUG DE QUE SI SE CAMBIA DE FASE, AVECES IT ES 0 Y NO ENTRA A ESTE IF.
    if (diff >= it->second.duration) {
        shots.erase(it);
        return false;
    }
    // solo renderizamos en frames 0, 2, 4 si es una animación de 5 frames (AK47)
    if (it->second.duration == 5 && diff % 2 != 0) {
        return false;
    }

    return true;  // activo durante los `duration` frames
}


SDL2pp::Point Animation::get_shot_impact(const std::string& username) const {
    return shots.at(username).impact_position;
}

void Animation::render_shot(SDL2pp::Point origin_camera, SDL2pp::Point end_world, GunType gun,
                            int angle) {
    SDL2pp::Rect viewport = camera.get_viewport();
    SDL2pp::Point origin_world = camera.point_screen_to_world(origin_camera);

    if (!viewport.IntersectLine(origin_world, end_world))
        return;

    SDL2pp::Point end_camera = camera.point_world_to_screen(end_world);

    double dx = static_cast<double>(end_camera.GetX() - origin_camera.GetX());
    double dy = static_cast<double>(end_camera.GetY() - origin_camera.GetY());
    double rad = std::atan2(dy, dx);

    if (gun == AWP) {
        const int thickness = 2;
        for (int i = -thickness; i <= thickness; ++i) {
            int offset_x = static_cast<int>(-std::sin(rad) * i);
            int offset_y = static_cast<int>(std::cos(rad) * i);

            SDL2pp::Point a(origin_camera.GetX() + offset_x, origin_camera.GetY() + offset_y);
            SDL2pp::Point b(end_camera.GetX() + offset_x, end_camera.GetY() + offset_y);

            renderer.SetDrawColor(255, 255, 0);
            renderer.DrawLine(a, b);
        }
    } else if (gun == GLOCK || gun == AK47) {
        renderer.SetDrawColor(255, 255, 0);
        renderer.DrawLine(origin_camera, end_camera);
    } else if (gun == M3) {
        const int num_lines = 8;
        const double spread_angle_deg = 20.0;
        const double length = 100.0;

        renderer.SetDrawColor(255, 255, 0);

        for (int i = 0; i < num_lines; ++i) {
            double offset = -spread_angle_deg / 2 + (spread_angle_deg / (num_lines - 1)) * i;
            double angle_rad = (angle + offset) * M_PI / 180.0;

            int dx_m3 = static_cast<int>(std::cos(angle_rad) * length);
            int dy_m3 = static_cast<int>(std::sin(angle_rad) * length);

            SDL2pp::Point end(origin_camera.GetX() + dx_m3, origin_camera.GetY() + dy_m3);
            renderer.DrawLine(origin_camera, end);
        }
    }

    if (gun == AWP || gun == M3 || gun == AK47) {
        std::string path = texture_parser.get_other_path(AWP_SHOT_FLARE);
        SDL2pp::Texture& flare_texture = texture_manager.get_texture(path);
        flare_texture.SetColorMod(255, 255, 0);
        flare_texture.SetBlendMode(SDL_BLENDMODE_ADD);
        SDL2pp::Rect rctdst(origin_camera.GetX() - 16, origin_camera.GetY() - 16, 32, 32);
        renderer.Copy(flare_texture, SDL2pp::NullOpt, rctdst);
    }

    renderer.SetDrawColor(0, 0, 0, 255);
}

void Animation::start_bomb_explosion(int current_it, SDL2pp::Point position_world) {
    if (!bomb_explosion.has_value()) {
        bomb_explosion = BombExplosionAnimation{current_it, position_world};
    }
}

bool Animation::is_bomb_explosion_active(int current_it) {
    if (!bomb_explosion.has_value())
        return false;

    int diff = current_it - bomb_explosion->start_it;
    if (diff >= bomb_explosion->duration) {
        bomb_explosion.reset();
        return false;
    }
    return true;
}


void Animation::render_bomb_explosion(int current_it) {
    if (!bomb_explosion.has_value())
        return;

    int frame = current_it - bomb_explosion->start_it;
    if (frame >= bomb_explosion->duration)
        return;

    int sprite_index = frame / 2;

    const SDL2pp::Point& pos_world = bomb_explosion->position_world;

    SDL2pp::Rect dst_world(pos_world.GetX() / GRAPHIC_SCALE - 128,
                           pos_world.GetY() / GRAPHIC_SCALE - 128, 256, 256);

    if (!camera.is_visible(dst_world))
        return;

    SDL2pp::Rect dst_camera = camera.rect_world_to_screen(dst_world);
    const BlockTextureInfo& frame_info = texture_parser.get_explosion_texture(sprite_index);
    SDL2pp::Texture& explosion_texture = texture_manager.get_texture(frame_info.tileset_path);
    SDL2pp::Rect src(frame_info.x, frame_info.y, frame_info.width, frame_info.height);
    renderer.Copy(explosion_texture, src, dst_camera);
}

void Animation::start_camera_shake(int current_it) {
    if (!camera_shake.has_value()) {
        camera_shake = CameraShake{current_it};
    }
}

bool Animation::is_camera_shake_active(int current_it) {
    if (!camera_shake.has_value())
        return false;
    int diff = current_it - camera_shake->start_it;
    if (diff >= camera_shake->duration) {
        camera_shake.reset();
        return false;
    }
    return true;
}

// quizas hacer un shake segun la distancia.
SDL2pp::Point Animation::get_camera_shake(int current_it) {
    if (!is_camera_shake_active(current_it)) {
        return SDL2pp::Point(0, 0);
    }

    int shake_intensity = 50;
    int dx = (std::rand() % (2 * shake_intensity + 1)) - shake_intensity;
    int dy = (std::rand() % (2 * shake_intensity + 1)) - shake_intensity;

    return SDL2pp::Point(dx, dy);
}

void Animation::start_damage_overlay(int current_it) {
    damage_overlay = DamageOverlayAnimation{current_it};
}

bool Animation::is_damage_overlay_active(int current_it) {
    if (!damage_overlay.has_value())
        return false;
    int diff = current_it - damage_overlay->start_it;
    if (diff >= damage_overlay->duration) {
        damage_overlay.reset();
        return false;
    }
    return true;
}

void Animation::render_damage_overlay(int current_it) {
    if (!is_damage_overlay_active(current_it))
        return;

    int frame = current_it - damage_overlay->start_it;
    int alpha =
            static_cast<int>(150 * (1.0 - frame / static_cast<float>(damage_overlay->duration)));

    std::string path = texture_parser.get_other_path(BLOOD_SCREEN);
    SDL2pp::Texture& texture = texture_manager.get_texture(path);

    texture.SetAlphaMod(alpha);
    SDL2pp::Rect full_screen(0, 0, CAMERA_WIDTH, CAMERA_HEIGHT);
    renderer.Copy(texture, SDL2pp::NullOpt, full_screen);
}

void Animation::restart_animations() {
    shots.clear();
    bomb_explosion.reset();
    camera_shake.reset();
    damage_overlay.reset();
}
