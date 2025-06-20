#ifndef ANIMATION_H
#define ANIMATION_H

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

public:
    Animation(SDL2pp::Renderer& renderer, Camera& camera, TextureManager& texture_manager,
              BlockTextureParser& texture_parser);

    void render_shot(SDL2pp::Point origin_camera, SDL2pp::Point end_world, GunType gun, int angle);
};

#endif
