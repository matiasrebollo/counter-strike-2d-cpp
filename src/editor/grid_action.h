#ifndef GRID_ACTION_H
#define GRID_ACTION_H

#include <utility>

class Game_editor;

class GridAction {
public:
    virtual ~GridAction() {}
    virtual void handle(std::pair<int, int> click_on, std::pair<int, int> click_drop,
                        Game_editor& editor) = 0;
    virtual void handle_delete(std::pair<int, int> click_on, std::pair<int, int> click_drop,
                               Game_editor& editor) = 0;
};

#endif
