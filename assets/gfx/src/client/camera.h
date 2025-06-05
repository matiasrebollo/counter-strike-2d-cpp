#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

class Camera {
private:
    int base_width;
    int base_height;
    int center_x;
    int center_y;

    SDL2pp::Rect get_viewport() const;

public:
    Camera(int screen_w, int screen_h);
    double calculate_angle_from_center(int mouse_x, int mouse_y) const;
    void follow(int x, int y);
    SDL2pp::Rect world_to_screen(const SDL2pp::Rect& rect_mundo) const;
    bool is_visible(const SDL2pp::Rect& rect_mundo) const;
};

#endif
