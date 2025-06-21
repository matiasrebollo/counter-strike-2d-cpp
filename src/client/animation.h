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

public:
    Animation(SDL2pp::Renderer& renderer, Camera& camera, TextureManager& texture_manager,
              BlockTextureParser& texture_parser);

    void start_shot(const std::string& username, int current_it, SDL2pp::Point impact,
                    int duration);
    bool is_shot_active(const std::string& username, int current_it);
    SDL2pp::Point get_shot_impact(const std::string& username) const;

    void render_shot(SDL2pp::Point origin_camera, SDL2pp::Point end_world, GunType gun, int angle);
};

#endif
