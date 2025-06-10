#ifndef GAME_EDITOR_H
#define GAME_EDITOR_H

#include <QMainWindow>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#include "../common/block_texture_parser.h"
#include "../common/game_map.h"

#include "blocks_setter.h"
#include "bomb_sites_setter.h"
#include "ct_spawns_setter.h"
#include "grid_action.h"
#include "tt_spawns_setter.h"

// cppcheck-suppress unknownMacro
QT_BEGIN_NAMESPACE
namespace Ui {
class Game_editor;
}
QT_END_NAMESPACE

class Game_editor: public QMainWindow {
    Q_OBJECT

public:
    explicit Game_editor(QWidget* parent = nullptr);
    ~Game_editor();
    void setupToolbar();
    void setBlock(const int& row, const int& colum);
    void setCtSpawn(const int& row, const int& colum);
    void setTTSpawn(const int& row, const int& colum);
    void setBombSite(const int& row, const int& colum);

private slots:
    void on_save_button_clicked();

    void on_go_to_create_button_clicked();

    void on_create_map_button_clicked();

private:
    Ui::Game_editor* ui;
    void setupUi(const int& rows, const int& columns);
    GameMap create_map(const std::vector<std::vector<int>>& grid);
    void setupBlockList();
    void setupBackgroundList();
    void setupGridMap(const int& rows, const int& colums);
    std::vector<Vector2D<int>> set_to_vector(const std::set<std::pair<int, int>>& set_pos);

    BlockTextureParser texture_parser;
    int selected_block;
    Background selected_background;
    std::vector<std::vector<int>> grid;
    std::unique_ptr<GridAction> mode;
    std::set<std::pair<int, int>> tt_spawns;
    std::set<std::pair<int, int>> ct_spawns;
    std::set<std::pair<int, int>> bomb_sites;
    std::pair<int, int> first_click;
    std::pair<int, int> second_click;
    bool first_click_done;
};
#endif  // GAME_EDITOR_H
