#include "game_editor.h"

#include "./ui_game_editor.h"

#include "clickablelabel.h"

Game_editor::Game_editor(QWidget* parent): QMainWindow(parent), ui(new Ui::Game_editor) {
    ui->setupUi(this);
    this->setupUi();
}

Game_editor::~Game_editor() { delete ui; }

void Game_editor::setupUi() {
    QMap<QString, QVector<int>> blocks_imgs = {{"box", {64, 128, 32, 32}},
                                               {"floor", {64, 32, 32, 32}}};

    QPixmap tileset(":images/tiles/default_dust.png");

    for (const QString& block: blocks_imgs.keys()) {
        ClickableLabel* label = new ClickableLabel();
        label->setFixedSize(60, 60);
        QVector<int> pos = blocks_imgs[block];
        QPixmap tile = tileset.copy(pos[0], pos[1], pos[2], pos[3]);
        label->setPixmap(tile.scaled(50, 50));
        ui->block_list->addWidget(label);

        connect(label, &ClickableLabel::clicked, [this, block]() { selectedBlock = block; });
    }

    int N = 50, M = 10;
    ui->scrollAreaWidgetContents_5->setMinimumSize(50 * M, 50 * N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            ClickableLabel* cell = new ClickableLabel();
            cell->setFixedSize(50, 50);
            connect(cell, &ClickableLabel::clicked, this, [this, cell]() {
                if (!selectedBlock.isEmpty()) {
                    QMap<QString, QVector<int>> blocks_imgs = {{"box", {64, 128, 32, 32}},
                                                               {"floor", {64, 32, 32, 32}}};
                    QPixmap tileset(":images/tiles/default_dust.png");
                    QVector<int> pos = blocks_imgs[selectedBlock];
                    QPixmap tile = tileset.copy(pos[0], pos[1], pos[2], pos[3]);
                    cell->setPixmap(tile.scaled(50, 50));
                }
            });
            ui->grid_map->addWidget(cell, i, j);
        }
    }
}
