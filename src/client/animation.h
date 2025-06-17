#ifndef ANIMATION_H
#define ANIMATION_H

#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "camera.h"

class Animation {
private:
    /*struct Shot {
        SDL2pp::Point origin;
        double angle;
        double distance;
    };*/

    SDL2pp::Renderer& renderer;
    Camera& camera;

    // std::unordered_map<std::string, Shot> active_shots; // luego quizas el valor debe ser un
    // vector de shots
public:
    Animation(SDL2pp::Renderer& renderer, Camera& camera);

    /*void start_shot(const std::string& username, SDL2pp::Point origin,
                    double angle, double distance);*/

    void render_shot(SDL2pp::Point origin, double angle, double distance);
};

#endif
