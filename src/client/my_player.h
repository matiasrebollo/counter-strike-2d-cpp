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

    int get_x_pos() const;
    int get_y_pos() const;
    std::string get_username() const;
};

#endif
