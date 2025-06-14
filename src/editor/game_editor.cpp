#include "game_editor.h"

#include <QFileDialog>
#include <QPainter>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../common/block_texture_parser.h"
#include "../common/yaml_parser.h"
#include "./ui_game_editor.h"

Game_editor::Game_editor(QWidget* parent):
        QMainWindow(parent),
        ui(new Ui::Game_editor),
        texture_parser(),
        pixmap_manager(),
        selected_block(NONE_BLOCK),
        selected_background(AZTEC_BACKGROUND),
        mode(std::make_unique<BlocksSetter>()),
        first_click_done(false) {
    ui->setupUi(this);
    ui->stack->setCurrentIndex(0);
}

Game_editor::~Game_editor() { delete ui; }

void Game_editor::setupUi() {
    this->setupToolbar();
    this->setupBlockList();
    this->setupBackgroundList();
    this->setupGridMap();
}

void Game_editor::setupBlockList() {
    QLayout* oldLayout = ui->scrollAreaWidgetContents->layout();
    if (oldLayout) {
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    auto* gridLayout = new QGridLayout(ui->scrollAreaWidgetContents);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setHorizontalSpacing(10);
    gridLayout->setVerticalSpacing(10);

    ui->scrollAreaWidgetContents->setLayout(gridLayout);

    const int columns = 3;
    int count_blocks = 0;

    for (const auto& block: texture_parser.get_blocks_keys()) {
        ClickableLabel* label = new ClickableLabel();
        label->setFixedSize(50, 50);

        BlockTextureInfo texture = texture_parser.get_texture_info(block);
        QPixmap& tile = pixmap_manager.get_block_pixmap(block);
        label->setPixmap(tile.scaled(50, 50));
        label->setCursor(Qt::CrossCursor);

        int row = count_blocks / columns;
        int col = count_blocks % columns;
        gridLayout->addWidget(label, row, col);

        if (texture.collidable) {
            mark_as_collidable(label);
        }

        connect(label, &ClickableLabel::clicked, [this, block]() {
            mode = std::make_unique<BlocksSetter>();
            selected_block = block;
            first_click_done = false;
        });

        count_blocks++;
    }
}


void Game_editor::setupToolbar() {
    ClickableLabel* labelTT = new ClickableLabel();
    ui->GameAreas->addStretch();
    labelTT->setFixedSize(60, 80);
    labelTT->setText("TT");
    labelTT->setCursor(Qt::CrossCursor);
    labelTT->setStyleSheet("color:white;"
                           "border-width: 1px;"
                           "border-style: solid;"
                           "border-color: white;");
    connect(labelTT, &ClickableLabel::clicked, [this]() {
        first_click_done = false;
        mode = std::make_unique<TTSpawnsSetter>();
    });

    ClickableLabel* labelCT = new ClickableLabel();
    labelCT->setFixedSize(60, 80);
    labelCT->setText("CT");
    labelCT->setCursor(Qt::CrossCursor);
    labelCT->setStyleSheet("color:white;"
                           "border-width: 1px;"
                           "border-style: solid;"
                           "border-color: white;");
    connect(labelCT, &ClickableLabel::clicked, [this]() {
        first_click_done = false;
        mode = std::make_unique<CTSpawnsSetter>();
    });

    ClickableLabel* labelBombSites = new ClickableLabel();
    labelBombSites->setFixedSize(60, 80);
    labelBombSites->setText("SITES");
    labelBombSites->setCursor(Qt::CrossCursor);
    labelBombSites->setStyleSheet("color:white;"
                                  "border-width: 1px;"
                                  "border-style: solid;"
                                  "border-color: white;");
    connect(labelBombSites, &ClickableLabel::clicked, [this]() {
        first_click_done = false;
        mode = std::make_unique<BombSiteSetter>();
    });
    ui->GameAreas->addWidget(labelTT, 0, Qt::AlignHCenter);
    ui->GameAreas->addWidget(labelCT, 0, Qt::AlignHCenter);
    ui->GameAreas->addWidget(labelBombSites, 0, Qt::AlignHCenter);
    ui->GameAreas->addStretch();
}

void Game_editor::setupBackgroundList() {
    for (const auto& background: texture_parser.get_backgrounds()) {
        ClickableLabel* label = new ClickableLabel();
        label->setFixedSize(60, 60);
        std::string background_path = texture_parser.get_background_path(background);
        QPixmap& background_image = pixmap_manager.get_pixmap(background_path);
        label->setPixmap(background_image.scaled(50, 50));
        ui->backgrounds_list->addWidget(label);
        label->setCursor(Qt::CrossCursor);
        connect(label, &ClickableLabel::clicked, [this, background, background_path]() {
            QString qss = QString("#scrollAreaGridMap {"
                                  "border-image: url(%1) 0 0 0 0 stretch stretch;"
                                  "}")
                                  .arg(QString::fromStdString(background_path));

            ui->scrollAreaGridMap->setStyleSheet(qss);
            ui->scrollAreaGridMap->setStyleSheet(qss);
            selected_background = background;
            first_click_done = false;
        });
    }
}

void Game_editor::setupGridMap() {
    int rows = 10;
    int columns = 10;

    ui->scrollAreaGridMap->setMinimumSize(50 * columns, 50 * rows);
    this->grid.resize(rows, std::vector<int>(columns, NONE_BLOCK));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            ClickableLabel* cell = new ClickableLabel();
            cell->setFixedSize(50, 50);
            QPixmap& base = pixmap_manager.get_block_pixmap(grid[i][j]);
            cell->setPixmap(base);
            connect(cell, &ClickableLabel::clicked, this, [this, cell, i, j]() {
                if (first_click_done) {
                    second_click = {j, i};
                    mode->handle(first_click, second_click, *this);
                    first_click_done = false;
                } else {
                    first_click = {j, i};
                    first_click_done = true;
                }
            });
            ui->grid_map->addWidget(cell, i, j);
        }
    }
}

void Game_editor::on_save_button_clicked() {
    GameMap map = create_map(grid);
    YamlParser parser;
    YAML::Node yaml = parser.game_map_to_Yaml(map);

    QString fileName = QFileDialog::getSaveFileName(
            this, "Guardar Mapa", "", "Archivos YAML (*.yaml);;Todos los archivos (*)");

    if (fileName.isEmpty()) {
        return;
    }

    if (!fileName.endsWith(".yaml", Qt::CaseInsensitive)) {
        fileName += ".yaml";
    }

    std::ofstream fout(fileName.toStdString());
    fout << yaml;
    close();
}

GameMap Game_editor::create_map(const std::vector<std::vector<int>>& grid) {
    int height = static_cast<int>(grid.size());
    int width = static_cast<int>(grid[0].size());

    int offset_x = width;
    int right_most = 0;
    int offset_y = height;
    int bottom_most = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int block = grid[i][j];
            if (block != NONE_BLOCK) {
                offset_x = std::min(j, offset_x);
                right_most = std::max(j, right_most);
                offset_y = std::min(i, offset_y);
                bottom_most = std::max(i, bottom_most);
            }
        }
    }

    std::vector<MapObject> blocks = load_blocks(offset_x, offset_y);
    std::vector<Vector2D<int>> ct_spawns_vector = set_to_vector(ct_spawns, offset_x, offset_y);
    std::vector<Vector2D<int>> tt_spawns_vector = set_to_vector(tt_spawns, offset_x, offset_x);
    std::vector<Vector2D<int>> sites_vector = set_to_vector(bomb_sites, offset_x, offset_y);

    return {right_most - offset_x + 1, bottom_most - offset_y + 1, selected_background, blocks,
            ct_spawns_vector,          tt_spawns_vector,           sites_vector};
}

std::vector<MapObject> Game_editor::load_blocks(const int& offset_x, const int& offset_y) {
    std::map<int, std::vector<Vector2D<int>>> positions_map;
    int height = static_cast<int>(grid.size());
    int width = static_cast<int>(grid[0].size());
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int block = grid[i][j];
            positions_map[block].push_back(Vector2D<int>(j - offset_x, i - offset_y));
        }
    }

    std::vector<MapObject> blocks;

    std::transform(positions_map.begin(), positions_map.end(), std::back_inserter(blocks),
                   [this](const auto& pair) {
                       return MapObject{pair.second, pair.first,
                                        texture_parser.get_texture_info(pair.first).collidable};
                   });
    return blocks;
}

std::vector<Vector2D<int>> Game_editor::set_to_vector(const std::set<std::pair<int, int>>& set_pos,
                                                      const int& offset_x, const int& offset_y) {
    std::vector<Vector2D<int>> vec;
    vec.reserve(set_pos.size());

    std::transform(set_pos.begin(), set_pos.end(), std::back_inserter(vec),
                   [offset_x, offset_y](const auto& pair) {
                       return Vector2D<int>(pair.first - offset_x, pair.second - offset_y);
                   });
    return vec;
}

void Game_editor::setBlock(const int& row, const int& column) {
    this->grid[row][column] = selected_block;
    this->render_block_info(row, column);
}

void Game_editor::setCtSpawn(const int& row, const int& column) {
    if (ct_spawns.find({column, row}) != ct_spawns.end()) {
        ct_spawns.erase({column, row});
    } else {
        ct_spawns.insert({column, row});
    }
    this->render_block_info(row, column);
}


void Game_editor::setTTSpawn(const int& row, const int& column) {
    if (tt_spawns.find({column, row}) != tt_spawns.end()) {
        tt_spawns.erase({column, row});
    } else {
        tt_spawns.insert({column, row});
    }
    this->render_block_info(row, column);
}

void Game_editor::setBombSite(const int& row, const int& column) {
    if (bomb_sites.find({column, row}) != bomb_sites.end()) {
        bomb_sites.erase({column, row});
    } else {
        bomb_sites.insert({column, row});
    }
    this->render_block_info(row, column);
}

void Game_editor::on_go_to_create_button_clicked() {
    this->setupUi();
    ui->stack->setCurrentIndex(1);
}

void Game_editor::on_add_columns_button_clicked() {
    int COLLUMNS_TO_ADD = 1;
    int rows_actual = grid.size();
    int collumns_actual = grid[0].size();

    ui->scrollAreaGridMap->setMinimumSize(50 * (collumns_actual + COLLUMNS_TO_ADD),
                                          50 * rows_actual);
    for (int i = 0; i < rows_actual; ++i) {
        this->grid[i].resize(collumns_actual + COLLUMNS_TO_ADD, NONE_BLOCK);
        for (int j = collumns_actual; j < collumns_actual + COLLUMNS_TO_ADD; ++j) {
            ClickableLabel* cell = new ClickableLabel();
            cell->setFixedSize(50, 50);
            QPixmap& base = pixmap_manager.get_block_pixmap(grid[i][j]);
            cell->setPixmap(base);
            connect(cell, &ClickableLabel::clicked, this, [this, cell, i, j]() {
                if (first_click_done) {
                    second_click = {j, i};
                    mode->handle(first_click, second_click, *this);
                    first_click_done = false;
                } else {
                    first_click = {j, i};
                    first_click_done = true;
                }
            });
            ui->grid_map->addWidget(cell, i, j);
        }
    }
}

void Game_editor::on_add_rows_button_clicked() {
    int ROWS_TO_ADD = 1;
    int rows_actual = grid.size();
    int collumns_actual = grid[0].size();

    this->grid.resize(rows_actual + ROWS_TO_ADD, std::vector<int>(collumns_actual, NONE_BLOCK));
    ui->scrollAreaGridMap->setMinimumSize(50 * collumns_actual, 50 * (rows_actual + ROWS_TO_ADD));

    for (int i = rows_actual; i < ROWS_TO_ADD + rows_actual; ++i) {
        for (int j = 0; j < collumns_actual; ++j) {
            ClickableLabel* cell = new ClickableLabel();
            cell->setFixedSize(50, 50);
            QPixmap& base = pixmap_manager.get_block_pixmap(grid[i][j]);
            cell->setPixmap(base);
            connect(cell, &ClickableLabel::clicked, this, [this, cell, i, j]() {
                if (first_click_done) {
                    second_click = {j, i};
                    mode->handle(first_click, second_click, *this);
                    first_click_done = false;
                } else {
                    first_click = {j, i};
                    first_click_done = true;
                }
            });
            ui->grid_map->addWidget(cell, i, j);
        }
    }
}

void Game_editor::render_block_info(const int& row, const int& column) {
    QLayoutItem* item = ui->grid_map->itemAtPosition(row, column);
    if (item) {
        QWidget* widget = item->widget();
        if (ClickableLabel* cell = qobject_cast<ClickableLabel*>(widget)) {
            this->render_block(cell, grid[row][column]);
            if (tt_spawns.find({column, row}) != tt_spawns.end()) {
                mark_as_tt_spawn(cell);
            }
            if (ct_spawns.find({column, row}) != ct_spawns.end()) {
                mark_as_ct_spawn(cell);
            }
            if (bomb_sites.find({column, row}) != bomb_sites.end()) {
                mark_as_bomb_site(cell);
            }
        }
    }
}

void Game_editor::mark_as_collidable(ClickableLabel* label) {
    QPixmap result = label->pixmap(Qt::ReturnByValue);
    QPainter painter(&result);

    QPixmap& overlay = pixmap_manager.get_pixmap("../assets/gfx/collidable.png");
    QPixmap scaledOverlay = overlay.scaled(20, 20);

    int x = result.width() - scaledOverlay.width();
    int y = 0;
    painter.drawPixmap(x, y, scaledOverlay);

    painter.end();
    label->setPixmap(result.scaled(50, 50));
}

void Game_editor::render_block(ClickableLabel* cell, const int& block) {
    QPixmap& tile = pixmap_manager.get_block_pixmap(block);
    cell->setPixmap(tile.scaled(50, 50));
}

void Game_editor::mark_as_tt_spawn(ClickableLabel* label) {
    QPixmap result = label->pixmap(Qt::ReturnByValue);
    QPainter painter(&result);

    QPixmap& overlay = pixmap_manager.get_pixmap("../assets/gfx/terrorist_logo.png");
    QPixmap scaledOverlay = overlay.scaled(20, 20);

    int x = result.width() - scaledOverlay.width();
    int y = result.height() - scaledOverlay.height();
    painter.drawPixmap(x, y, scaledOverlay);

    painter.end();
    label->setPixmap(result.scaled(50, 50));
}

void Game_editor::mark_as_ct_spawn(ClickableLabel* label) {
    QPixmap result = label->pixmap(Qt::ReturnByValue);
    QPainter painter(&result);

    QPixmap& overlay = pixmap_manager.get_pixmap("../assets/gfx/counter_terrorist_logo.png");
    QPixmap scaledOverlay = overlay.scaled(20, 20);

    int x = 0;
    int y = result.height() - scaledOverlay.height();
    painter.drawPixmap(x, y, scaledOverlay);

    painter.end();
    label->setPixmap(result.scaled(50, 50));
}

void Game_editor::mark_as_bomb_site(ClickableLabel* label) {
    QPixmap result = label->pixmap(Qt::ReturnByValue);
    QPainter painter(&result);

    QPixmap& overlay = pixmap_manager.get_pixmap("../assets/gfx/weapons/bomb.bmp");
    QPixmap scaledOverlay = overlay.scaled(20, 20);

    int x = 0;
    int y = 0;
    painter.drawPixmap(x, y, scaledOverlay);

    painter.end();
    label->setPixmap(result.scaled(50, 50));
}
