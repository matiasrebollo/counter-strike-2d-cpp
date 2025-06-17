#include "animation.h"

#include <cmath>

Animation::Animation(SDL2pp::Renderer& renderer, Camera& camera):
        renderer(renderer), camera(camera) {}


void Animation::render_shot(SDL2pp::Point origin_camera, double angle, double distance) {
    double rad = angle * M_PI / 180.0;

    // viewport es en mundo
    SDL2pp::Rect viewport = camera.get_viewport();

    // Convertí origin_camera a mundo solo para calcular el end
    SDL2pp::Point origin_world = camera.point_screen_to_world(origin_camera);
    SDL2pp::Point end_world(static_cast<int>(origin_world.GetX() + std::cos(rad) * distance),
                            static_cast<int>(origin_world.GetY() + std::sin(rad) * distance));

    if (!viewport.IntersectLine(origin_world, end_world))
        return;
    SDL2pp::Point end_camera = camera.point_world_to_screen(end_world);

    renderer.SetDrawColor(255, 255, 0);
    renderer.DrawLine(origin_camera, end_camera);
    renderer.SetDrawColor(0, 0, 0, 255);
}
