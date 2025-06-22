#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/block_texture_parser.h"
#include "../common/weapon_type.h"

#include "camera.h"
#include "texture_manager.h"

class Animation {
private:
    SDL2pp::Renderer& renderer;
    Camera& camera;
    TextureManager& texture_manager;
    BlockTextureParser& texture_parser;

    struct ShotAnimation {
        int start_it;
        SDL2pp::Point impact_position;
        int duration;  // en frames
    };
    std::unordered_map<std::string, ShotAnimation> shots;

    struct BombExplosionAnimation {
        int start_it;
        SDL2pp::Point position_world;
        int duration = 50;
    };

    std::optional<BombExplosionAnimation> bomb_explosion;

    struct CameraShake {
        int start_it;
        int duration = 35;
    };
    std::optional<CameraShake> camera_shake;

    struct DamageOverlayAnimation {
        int start_it;
        int duration = 50;
    };

    std::optional<DamageOverlayAnimation> damage_overlay;

public:
    Animation(SDL2pp::Renderer& renderer, Camera& camera, TextureManager& texture_manager,
              BlockTextureParser& texture_parser);

    void start_shot(const std::string& username, int current_it, SDL2pp::Point impact,
                    int duration);
    bool is_shot_active(const std::string& username, int current_it);
    SDL2pp::Point get_shot_impact(const std::string& username) const;
    void render_shot(SDL2pp::Point origin_camera, SDL2pp::Point end_world, GunType gun, int angle);

    void start_bomb_explosion(int current_it, SDL2pp::Point position_world);
    bool is_bomb_explosion_active(int current_it);
    void render_bomb_explosion(int current_it);

    void start_camera_shake(int current_it);
    bool is_camera_shake_active(int current_it);
    SDL2pp::Point get_camera_shake(int current_it);

    void start_damage_overlay(int current_it);
    bool is_damage_overlay_active(int current_it);
    void render_damage_overlay(int current_it);
};

#endif
