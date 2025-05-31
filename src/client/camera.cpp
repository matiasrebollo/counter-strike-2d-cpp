#include "camera.h"


Camera::Camera(int screen_w, int screen_h):
        screen_width(screen_w), screen_height(screen_h), center_x(0), center_y(0) {}

SDL2pp::Rect Camera::get_viewport() const {
    return SDL2pp::Rect((center_x - screen_width / 2), (center_y - screen_height / 2), screen_width,
                        screen_height);
}

double Camera::calculate_angle_from_center(int mouse_x, int mouse_y) const {
    float dx = mouse_x - static_cast<float>(screen_width / 2.0f);
    float dy = mouse_y - static_cast<float>(screen_height / 2.0f);
    float ang_radianes = atan2(dy, dx);
    return (ang_radianes * 180.0f / M_PI) + 90;
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
