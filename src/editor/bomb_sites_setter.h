#ifndef BOMB_SITES_SETTER_H
#define BOMB_SITES_SETTER_H

#include <utility>

#include "game_editor.h"
#include "grid_action.h"

class Game_editor;

class BombSiteSetter: public GridAction {
    void handle(std::pair<int, int> click_on, std::pair<int, int> click_drop, Game_editor& editor,
                const bool& to_delete) override;
};

#endif
