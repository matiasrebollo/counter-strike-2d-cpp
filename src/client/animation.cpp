#include "animation.h"

#include <cmath>

Animation::Animation(SDL2pp::Renderer& renderer): renderer(renderer) {}

/*void Animation::start_shot(const std::string& username, SDL2pp::Point origin,
                           double angle, double distance) {
    active_shots[username] = Shot{origin, angle, distance};
}*/

void Animation::render_shot(SDL2pp::Point origin, double angle, double distance) {
    renderer.SetDrawColor(255, 255, 0);  // Amarillo para la Glock

    std::cout << "distance: " << distance << std::endl;

    double rad = angle * M_PI / 180.0;
    SDL2pp::Point end(static_cast<int>(origin.GetX() + std::cos(rad) * distance),
                      static_cast<int>(origin.GetY() + std::sin(rad) * distance));

    renderer.DrawLine(origin, end);

    renderer.SetDrawColor(0, 0, 0, 255);
}
