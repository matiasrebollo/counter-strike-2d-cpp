#include "camera.h"


Camera::Camera(int screen_w, int screen_h):
        base_width(screen_w), base_height(screen_h), center_x(0), center_y(0) {}


int Camera::get_base_width() const { return base_width; }
int Camera::get_base_height() const { return base_height; }


SDL2pp::Rect Camera::get_viewport() const {
    return SDL2pp::Rect((center_x - base_width / 2), (center_y - base_height / 2), base_width,
                        base_height);
}

void Camera::follow(int x, int y) {
    center_x = x;
    center_y = y;
}

SDL2pp::Rect Camera::world_to_screen(const SDL2pp::Rect& rect_mundo) const {
    SDL2pp::Rect viewport = get_viewport();
    return SDL2pp::Rect(rect_mundo.GetX() - viewport.GetX(), rect_mundo.GetY() - viewport.GetY(),
                        rect_mundo.GetW(), rect_mundo.GetH());
}

bool Camera::is_visible(const SDL2pp::Rect& rect_mundo) const {
    SDL2pp::Rect viewport = get_viewport();
    return viewport.Intersects(rect_mundo);
}
