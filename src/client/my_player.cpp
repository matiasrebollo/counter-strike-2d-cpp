#include "my_player.h"

#include <cmath>

MyPlayer::MyPlayer(const std::string& username): username(username), x_pos(0), y_pos(0) {}

void MyPlayer::update_my_position(const PlayerDTO& p) {
    if (p.username == this->username) {
        x_pos = p.position.x;
        y_pos = p.position.y;
    }
}

double MyPlayer::calculate_angle(int mouse_x, int mouse_y) const {
    float dx = mouse_x - static_cast<float>(x_pos + 16);
    float dy = mouse_y - static_cast<float>(y_pos + 16);
    float ang_radianes = atan2(dy, dx);
    double angulo = (ang_radianes * 180.0f / M_PI) + 90;
    return angulo;
}
