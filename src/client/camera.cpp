#include "camera.h"


Camera::Camera(int screen_w, int screen_h):
        base_width(screen_w), base_height(screen_h), center_x(0), center_y(0) {}


SDL2pp::Rect Camera::get_viewport() const {
    return SDL2pp::Rect((center_x - base_width / 2), (center_y - base_height / 2), base_width,
                        base_height);
}

void Camera::follow(int x, int y) {
    center_x = x;
    center_y = y;
}

SDL2pp::Point Camera::point_screen_to_world(const SDL2pp::Point& p) const {
    SDL2pp::Rect viewport = get_viewport();
    return SDL2pp::Point(p.GetX() + viewport.GetX(), p.GetY() + viewport.GetY());
}

SDL2pp::Point Camera::point_world_to_screen(const SDL2pp::Point& p) const {
    SDL2pp::Rect viewport = get_viewport();
    return SDL2pp::Point(p.GetX() - viewport.GetX(), p.GetY() - viewport.GetY());
}


SDL2pp::Rect Camera::rect_world_to_screen(const SDL2pp::Rect& rect_mundo) const {
    SDL2pp::Rect viewport = get_viewport();
    return SDL2pp::Rect(rect_mundo.GetX() - viewport.GetX(), rect_mundo.GetY() - viewport.GetY(),
                        rect_mundo.GetW(), rect_mundo.GetH());
}

bool Camera::is_visible(const SDL2pp::Rect& rect_mundo) const {
    SDL2pp::Rect viewport = get_viewport();
    return viewport.Intersects(rect_mundo);
}
