#include "bomb_sites_setter.h"

#include <algorithm>
#include <utility>

#include "game_editor.h"

void BombSiteSetter::handle(std::pair<int, int> click_on, std::pair<int, int> click_drop,
                            Game_editor& editor) {
    int x1 = std::min(click_on.first, click_drop.first);
    int y1 = std::min(click_on.second, click_drop.second);
    int x2 = std::max(click_on.first, click_drop.first);
    int y2 = std::max(click_on.second, click_drop.second);

    for (int i = y1; i <= y2; ++i) {
        for (int j = x1; j <= x2; ++j) {
            editor.setBombSite(i, j, false);
        }
    }
}


void BombSiteSetter::handle_delete(std::pair<int, int> click_on, std::pair<int, int> click_drop,
                                   Game_editor& editor) {
    int x1 = std::min(click_on.first, click_drop.first);
    int y1 = std::min(click_on.second, click_drop.second);
    int x2 = std::max(click_on.first, click_drop.first);
    int y2 = std::max(click_on.second, click_drop.second);

    for (int i = y1; i <= y2; ++i) {
        for (int j = x1; j <= x2; ++j) {
            editor.setBombSite(i, j, true);
        }
    }
}
