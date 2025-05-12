#ifndef CS2D_GAME_H
#define CS2D_GAME_H

#include <vector>

#include "server/collidable.h"
#include "server/player.h"

class CS2DGame {
private:
    std::vector<Player> players;
    std::vector<Collidable> map_objects;
    bool running = true;

public:
    CS2DGame();

    void add_player();

    void game_loop();

    void print_map_objects() const;

    CS2DGame(const CS2DGame&) = delete;
    CS2DGame& operator=(const CS2DGame&) = delete;

    ~CS2DGame();
};

#endif
