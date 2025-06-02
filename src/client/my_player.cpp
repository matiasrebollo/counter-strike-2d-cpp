#include "my_player.h"

#include <cmath>

MyPlayer::MyPlayer(const std::string& username): username(username), x_pos(0), y_pos(0) {}

void MyPlayer::update_my_position(const PlayerDTO& p) {
    if (p.username == this->username) {
        x_pos = p.position.x;
        y_pos = p.position.y;
    }
}


int MyPlayer::get_x_pos() const { return this->x_pos; }

int MyPlayer::get_y_pos() const { return this->y_pos; }

std::string MyPlayer::get_username() const { return this->username; }
