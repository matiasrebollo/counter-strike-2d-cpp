#ifndef GAME_EDITOR_H
#define GAME_EDITOR_H

#include <QMainWindow>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "../common/block_texture_parser.h"
#include "../common/game_map.h"
#include "../common/weapon_type.h"
#include "grid_action/blocks_setter.h"
#include "grid_action/bomb_sites_setter.h"
#include "grid_action/ct_spawns_setter.h"
#include "grid_action/grid_action.h"
#include "grid_action/guns_setter.h"
#include "grid_action/tt_spawns_setter.h"

#include "clickable_label.h"
#include "logic_map.h"
#include "pixmap_manager.h"

#define TITLE_MSG_EDIT "Editar una partida"
#define MSG_MAP_NOT_SELECTED "Debe seleccionar un mapa para crear una partida."

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
    void setBlock(const int& row, const int& column, const bool& to_delete);
    void setCtSpawn(const int& row, const int& column, const bool& to_delete);
    void setTTSpawn(const int& row, const int& column, const bool& to_delete);
    void setBombSite(const int& row, const int& column, const bool& to_delete);
    void setGun(const int& row, const int& column, const bool& to_delete);

private slots:
    void on_save_button_clicked();
    void on_load_map_clicked();
    void on_back_button_clicked();
    void on_back_button_2_clicked();
    void on_load_map_button_clicked();
    void on_go_to_create_button_clicked();
    void on_add_columns_button_clicked();
    void on_add_rows_button_clicked();

private:
    Ui::Game_editor* ui;
    void setupEditorUi();
    GameMap create_map(const std::vector<std::vector<int>>& grid);
    void setupBlockList();
    void setupBackgroundList();
    void setupGridMap();
    void setupGunBar();
    void add_grid_map_cell(const int& i, const int& j);
    void clear_grid_map();
    void load_map_from_file(const std::string& map_name);
    void clear_grid();
    void onBackgroundLabelClicked(const Background& background, const std::string& background_path);
    void render_block_info(const int& row, const int& column);
    void render_block(ClickableLabel* cell, const int& block);
    void mark_as_collidable(ClickableLabel* label);
    void mark_as_ct_spawn(ClickableLabel* label);
    void mark_as_tt_spawn(ClickableLabel* label);
    void mark_as_bomb_site(ClickableLabel* label);
    void mark_with_gun(ClickableLabel* label, const GunType& gun);
    void format_string(std::string& s);
    BlockTextureParser texture_parser;
    PixmapManager pixmap_manager;
    LogicMap logic_map;
    std::unique_ptr<GridAction> mode;
    std::pair<int, int> first_left_click;
    std::pair<int, int> second_left_click;
    std::pair<int, int> first_right_click;
    std::pair<int, int> second_right_click;
    bool first_left_click_done;
    bool first_right_click_done;
    bool has_entry_create;
};

#endif  // GAME_EDITOR_H
