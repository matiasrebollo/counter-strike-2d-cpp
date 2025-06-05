#ifndef GAME_EDITOR_H
#define GAME_EDITOR_H

#include <QMainWindow>
#include <vector>

#include "../common/block_texture_parser.h"
#include "../common/game_map.h"

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

private slots:
    void on_save_button_clicked();

private:
    Ui::Game_editor* ui;
    void setupUi();
    GameMap create_map(const std::vector<std::vector<int>>& grid);


    BlockTextureParser texture_parser;
    int selected_block;
    std::vector<std::vector<int>> grid;
};
#endif  // GAME_EDITOR_H
