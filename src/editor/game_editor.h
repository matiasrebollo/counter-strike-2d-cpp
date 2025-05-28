#ifndef GAME_EDITOR_H
#define GAME_EDITOR_H

#include <QMainWindow>

#include "../common/block_texture_parser.h"

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

private:
    Ui::Game_editor* ui;
    void setupUi();

    BlockTextureParser texture_parser;
    MapObjectType selected_block;
};
#endif  // GAME_EDITOR_H
