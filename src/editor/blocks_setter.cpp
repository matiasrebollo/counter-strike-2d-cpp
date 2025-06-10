#include "blocks_setter.h"

#include <QDebug>
#include <algorithm>
#include <utility>

#include "game_editor.h"

void BlocksSetter::handle(std::pair<int, int> click_on, std::pair<int, int> click_drop,
                          Game_editor& editor) {
    int x1 = std::min(click_on.first, click_drop.first);
    int y1 = std::min(click_on.second, click_drop.second);
    int x2 = std::max(click_on.first, click_drop.first);
    int y2 = std::max(click_on.second, click_drop.second);

    for (int i = y1; i <= y2; ++i) {
        for (int j = x1; j <= x2; ++j) {
            editor.setBlock(i, j);
        }
    }
}
