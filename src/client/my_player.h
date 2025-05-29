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

    int get_x_pos() const;
    int get_y_pos() const;
    std::string get_username() const;
    // quizas agregar funciones que me den el centro del jugador, y los atributos ancho y altura.
};

#endif
