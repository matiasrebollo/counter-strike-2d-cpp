#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "common/settings.h"

class Camera {
private:
    int base_width;
    int base_height;
    int center_x;
    int center_y;

public:
    Camera(int screen_w, int screen_h);
    SDL2pp::Rect get_viewport() const;
    void follow(int x, int y);
    SDL2pp::Point point_screen_to_world(const SDL2pp::Point& p) const;
    SDL2pp::Point point_world_to_screen(const SDL2pp::Point& p) const;
    SDL2pp::Rect rect_world_to_screen(const SDL2pp::Rect& rect_mundo) const;
    bool is_visible(const SDL2pp::Rect& rect_mundo) const;
};

#endif
