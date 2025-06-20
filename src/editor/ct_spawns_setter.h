#ifndef CT_SPAWNS_H
#define CT_SPAWNS_H

#include <utility>

#include "game_editor.h"
#include "grid_action.h"

class Game_editor;

class CTSpawnsSetter: public GridAction {
    void handle(std::pair<int, int> click_on, std::pair<int, int> click_drop, Game_editor& editor,
                const bool& to_delete) override;
};

#endif
