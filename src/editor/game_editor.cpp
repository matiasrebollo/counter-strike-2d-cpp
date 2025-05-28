#include "game_editor.h"

#include <string>

#include "../common/block_texture_parser.h"
#include "./ui_game_editor.h"

#include "clickablelabel.h"

Game_editor::Game_editor(QWidget* parent):
        QMainWindow(parent), ui(new Ui::Game_editor), texture_parser(), selected_block(NONE_BLOCK) {
    ui->setupUi(this);
    this->setupUi();
}

Game_editor::~Game_editor() { delete ui; }

void Game_editor::setupUi() {
    for (const MapObjectType& block: texture_parser.get_keys()) {
        ClickableLabel* label = new ClickableLabel();
        label->setFixedSize(60, 60);
        BlockTextureInfo texture = texture_parser.get_texture_info(block);
        std::string path = ":images/tiles/" + texture.tileset_path;
        QPixmap tileset(QString::fromStdString(path));
        QPixmap tile = tileset.copy(texture.x, texture.y, texture.width, texture.height);
        label->setPixmap(tile.scaled(50, 50));
        ui->block_list->addWidget(label);

        connect(label, &ClickableLabel::clicked, [this, block]() { selected_block = block; });
    }

    int N = 10, M = 10;
    ui->scrollAreaWidgetContents_5->setMinimumSize(50 * M, 50 * N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            ClickableLabel* cell = new ClickableLabel();
            cell->setFixedSize(50, 50);
            connect(cell, &ClickableLabel::clicked, this, [this, cell]() {
                if (selected_block != NONE_BLOCK) {
                    BlockTextureInfo texture = texture_parser.get_texture_info(selected_block);
                    std::string path = ":images/tiles/" + texture.tileset_path;
                    QPixmap tileset(QString::fromStdString(path));
                    QPixmap tile =
                            tileset.copy(texture.x, texture.y, texture.width, texture.height);
                    cell->setPixmap(tile.scaled(50, 50));
                }
            });
            ui->grid_map->addWidget(cell, i, j);
        }
    }
}
