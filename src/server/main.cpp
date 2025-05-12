#include "server/cs2d_game.h"

int main() {
    CS2DGame game;
    game.add_player();
    game.game_loop();
    return 0;
}
