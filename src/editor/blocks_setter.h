#ifndef BLOCKS_SETTER_H
#define BLOCKS_SETTER_H

#include <utility>

#include "game_editor.h"
#include "grid_action.h"

class Game_editor;

class BlocksSetter: public GridAction {
    void handle(std::pair<int, int> click_on, std::pair<int, int> click_drop,
                Game_editor& editor) override;

    void handle_delete(std::pair<int, int> click_on, std::pair<int, int> click_drop,
                       Game_editor& editor) override;
};

#endif
