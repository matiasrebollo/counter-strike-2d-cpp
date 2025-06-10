#include "game_editor.h"

#include <QFileDialog>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../common/block_texture_parser.h"
#include "../common/yaml_parser.h"
#include "./ui_game_editor.h"

#include "clickablelabel.h"

Game_editor::Game_editor(QWidget* parent):
        QMainWindow(parent),
        ui(new Ui::Game_editor),
        texture_parser(),
        selected_block(NONE_BLOCK),
        selected_background(AZTEC_BACKGROUND),
        mode(std::make_unique<BlocksSetter>()),
        first_click_done(false) {
    ui->setupUi(this);
    ui->stack->setCurrentIndex(0);
}

Game_editor::~Game_editor() { delete ui; }

void Game_editor::setupUi(const int& rows, const int& columns) {
    this->setupToolbar();
    this->setupBlockList();
    this->setupBackgroundList();
    this->setupGridMap(rows, columns);
}

void Game_editor::setupBlockList() {
    for (const auto& block: texture_parser.get_blocks_keys()) {
        ClickableLabel* label = new ClickableLabel();
        label->setFixedSize(50, 50);
        BlockTextureInfo texture = texture_parser.get_texture_info(block);
        QPixmap tileset(QString::fromStdString(texture.tileset_path));
        QPixmap tile = tileset.copy(texture.x, texture.y, texture.width, texture.height);
        label->setPixmap(tile.scaled(50, 50));
        ui->block_list->addWidget(label);

        connect(label, &ClickableLabel::clicked, [this, block]() {
            mode = std::make_unique<BlocksSetter>();
            selected_block = block;
            first_click_done = false;
        });
    }
}

void Game_editor::setupToolbar() {
    ClickableLabel* labelTT = new ClickableLabel();
    labelTT->setFixedSize(40, 40);
    ui->horizontalLayout_2->addWidget(labelTT);
    labelTT->setText("TT");
    connect(labelTT, &ClickableLabel::clicked, [this]() {
        first_click_done = false;
        mode = std::make_unique<TTSpawnsSetter>();
    });

    ClickableLabel* labelCT = new ClickableLabel();
    labelCT->setFixedSize(40, 40);
    ui->horizontalLayout_2->addWidget(labelCT);
    labelCT->setText("CT");
    connect(labelCT, &ClickableLabel::clicked, [this]() {
        first_click_done = false;
        mode = std::make_unique<CTSpawnsSetter>();
    });

    ClickableLabel* labelBombSites = new ClickableLabel();
    labelBombSites->setFixedSize(40, 40);
    ui->horizontalLayout_2->addWidget(labelBombSites);
    labelBombSites->setText("SITES");
    connect(labelBombSites, &ClickableLabel::clicked, [this]() {
        first_click_done = false;
        mode = std::make_unique<BombSiteSetter>();
    });
}

void Game_editor::setupBackgroundList() {
    for (const auto& background: texture_parser.get_backgrounds()) {
        ClickableLabel* label = new ClickableLabel();
        label->setFixedSize(60, 60);
        std::string background_path = texture_parser.get_background_path(background);
        QPixmap background_image(QString::fromStdString(background_path));
        label->setPixmap(background_image.scaled(50, 50));
        ui->backgrounds_list->addWidget(label);
        connect(label, &ClickableLabel::clicked, [this, background, background_path]() {
            ui->scrollAreaGridMap->setStyleSheet("background-image: url(" +
                                                 QString::fromStdString(background_path) +
                                                 ");"
                                                 "background-repeat: no-repeat;"
                                                 "background-position: center;");
            selected_background = background;
            first_click_done = false;
        });
    }
}

void Game_editor::setupGridMap(const int& rows, const int& colums) {
    ui->scrollAreaGridMap->setMinimumSize(50 * colums, 50 * rows);
    this->grid.resize(rows);
    for (int i = 0; i < rows; ++i) {
        this->grid[i].resize(colums, NONE_BLOCK);
        for (int j = 0; j < colums; ++j) {
            ClickableLabel* cell = new ClickableLabel();
            cell->setFixedSize(50, 50);
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
    std::map<int, std::vector<Vector2D<int>>> positions_map;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int block = grid[i][j];
            positions_map[block].push_back(Vector2D<int>(j, i));
        }
    }

    std::vector<MapObject> blocks;

    std::transform(positions_map.begin(), positions_map.end(), std::back_inserter(blocks),
                   [this](const auto& pair) {
                       return MapObject{pair.second, pair.first,
                                        texture_parser.get_texture_info(pair.first).collidable};
                   });


    std::vector<Vector2D<int>> ct_spawns_vector = set_to_vector(ct_spawns);
    std::vector<Vector2D<int>> tt_spawns_vector = set_to_vector(tt_spawns);
    std::vector<Vector2D<int>> sites_vector = set_to_vector(bomb_sites);
  
    GameMap game_map = {width,       height,           selected_background,
                        blocks,      ct_spawns_vector, tt_spawns_vector,
                        sites_vector};
    return game_map;
}

std::vector<Vector2D<int>> Game_editor::set_to_vector(
        const std::set<std::pair<int, int>>& set_pos) {
    std::vector<Vector2D<int>> vec;
    vec.reserve(set_pos.size());

    std::transform(set_pos.begin(), set_pos.end(), std::back_inserter(vec),
                   [](const auto& pair) { return Vector2D<int>(pair.first, pair.second); });
    return vec;
}

void Game_editor::setBlock(const int& row, const int& colum) {
    QLayoutItem* item = ui->grid_map->itemAtPosition(row, colum);
    if (item) {
        QWidget* widget = item->widget();
        if (ClickableLabel* cell = qobject_cast<ClickableLabel*>(widget)) {
            if (selected_block != NONE_BLOCK) {
                BlockTextureInfo texture = texture_parser.get_texture_info(selected_block);
                std::string path = texture.tileset_path;
                QPixmap tileset(QString::fromStdString(path));
                QPixmap tile = tileset.copy(texture.x, texture.y, texture.width, texture.height);
                cell->setPixmap(tile.scaled(50, 50));
            }
        }
    }
    this->grid[row][colum] = selected_block;
}

void Game_editor::setCtSpawn(const int& row, const int& colum) {
    QLayoutItem* item = ui->grid_map->itemAtPosition(row, colum);
    if (item) {
        QWidget* widget = item->widget();
        if (ClickableLabel* cell = qobject_cast<ClickableLabel*>(widget)) {
            if (ct_spawns.find({colum, row}) != ct_spawns.end()) {
                cell->setStyleSheet("background-color: transparent;");
                ct_spawns.erase({colum, row});
            } else {
                cell->setStyleSheet("background-color: rgba(0, 0, 255, 60);");
                ct_spawns.insert({colum, row});
            }
        }
    }
}

void Game_editor::setTTSpawn(const int& row, const int& colum) {
    QLayoutItem* item = ui->grid_map->itemAtPosition(row, colum);
    if (item) {
        QWidget* widget = item->widget();
        if (ClickableLabel* cell = qobject_cast<ClickableLabel*>(widget)) {
            if (tt_spawns.find({colum, row}) != tt_spawns.end()) {
                cell->setStyleSheet("background-color: transparent;");
                tt_spawns.erase({colum, row});
            } else {
                cell->setStyleSheet("background-color: rgba(255, 255, 0, 60);");
                tt_spawns.insert({colum, row});
            }
        }
    }
}

void Game_editor::setBombSite(const int& row, const int& colum) {
    QLayoutItem* item = ui->grid_map->itemAtPosition(row, colum);
    if (item) {
        QWidget* widget = item->widget();
        if (ClickableLabel* cell = qobject_cast<ClickableLabel*>(widget)) {
            if (bomb_sites.find({colum, row}) != bomb_sites.end()) {
                cell->setStyleSheet("background-color: transparent;");
                bomb_sites.erase({colum, row});
            } else {
                cell->setStyleSheet("background-color: rgba(255, 0, 0, 60);");
                bomb_sites.insert({colum, row});
            }
        }
    }
}

void Game_editor::on_go_to_create_button_clicked() { ui->stack->setCurrentIndex(1); }


void Game_editor::on_create_map_button_clicked() {
    bool ok;
    QString cols = ui->columns_input->text();
    int columns = cols.toInt(&ok);
    if (not ok) {
        // mesaje error
        return;
    }

    QString rows_str = ui->rows_input->text();
    int rows = rows_str.toInt(&ok);
    if (not ok) {
        // mesaje error
        return;
    }

    this->setupUi(rows, columns);
    ui->stack->setCurrentIndex(2);
}
