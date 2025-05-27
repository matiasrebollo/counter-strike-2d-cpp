#ifndef MYPLAYER_H
#define MYPLAYER_H

#include <string>

#include "../common/player_dto.h"

class MyPlayer {
private:
    std::string username;
    int x_pos;
    int y_pos;

public:
    explicit MyPlayer(const std::string& username);

    void update_my_position(const PlayerDTO& p);
    double calculate_angle(int mouse_x, int mouse_y) const;
};

#endif
