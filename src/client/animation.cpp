#include "animation.h"

#include <cmath>
#include <string>

Animation::Animation(SDL2pp::Renderer& renderer, Camera& camera, TextureManager& texture_manager,
                     BlockTextureParser& texture_parser):
        renderer(renderer),
        camera(camera),
        texture_manager(texture_manager),
        texture_parser(texture_parser) {}

void Animation::render_shot(SDL2pp::Point origin_camera, SDL2pp::Point end_world, GunType gun) {
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
        std::string path = texture_parser.get_other_path(AWP_SHOT_FLARE);
        SDL2pp::Texture& flare_texture = texture_manager.get_texture(path);
        flare_texture.SetColorMod(255, 255, 0);
        flare_texture.SetBlendMode(SDL_BLENDMODE_ADD);
        SDL2pp::Rect rctdst(origin_camera.GetX() - 16, origin_camera.GetY() - 16, 32, 32);
        renderer.Copy(flare_texture, SDL2pp::NullOpt, rctdst);
    } else if (gun == GLOCK) {
        renderer.SetDrawColor(255, 255, 0);
        renderer.DrawLine(origin_camera, end_camera);
    }

    renderer.SetDrawColor(0, 0, 0, 255);
}
