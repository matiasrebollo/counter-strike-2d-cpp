#include "game_editor.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

#include "../common/block_texture_parser.h"
#include "../common/yaml_parser.h"
#include "./ui_game_editor.h"

#include "clickablelabel.h"

Game_editor::Game_editor(QWidget* parent):
        QMainWindow(parent), ui(new Ui::Game_editor), texture_parser(), selected_block(NONE_BLOCK) {
    ui->setupUi(this);
    this->setupUi();
}

Game_editor::~Game_editor() { delete ui; }

void Game_editor::setupUi() {
    for (const auto& block: texture_parser.get_keys()) {
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

    int N = 15, M = 20;
    ui->scrollAreaWidgetContents_5->setMinimumSize(50 * M, 50 * N);
    this->grid.resize(N);
    for (int i = 0; i < N; ++i) {
        this->grid[i].resize(M, NONE_BLOCK);
        for (int j = 0; j < M; ++j) {
            ClickableLabel* cell = new ClickableLabel();
            cell->setFixedSize(50, 50);
            connect(cell, &ClickableLabel::clicked, this, [this, cell, i, j]() {
                if (selected_block != NONE_BLOCK) {
                    BlockTextureInfo texture = texture_parser.get_texture_info(selected_block);
                    std::string path = "../assets/gfx/tiles/" + texture.tileset_path;
                    QPixmap tileset(QString::fromStdString(path));
                    QPixmap tile =
                            tileset.copy(texture.x, texture.y, texture.width, texture.height);
                    cell->setPixmap(tile.scaled(50, 50));
                }
                this->grid[i][j] = selected_block;
            });
            ui->grid_map->addWidget(cell, i, j);
        }
    }
}

void Game_editor::on_save_button_clicked() {
    GameMap map = create_map(grid);
    YamlParser parser;
    YAML::Node yaml = parser.game_map_to_Yaml(map);
    std::ofstream fout("../mapa.yaml");
    fout << yaml;
    close();
}

GameMap Game_editor::create_map(const std::vector<std::vector<int>>& grid) {
    int height = static_cast<int>(grid.size());
    int width = static_cast<int>(grid[0].size());
    std::map<int, std::vector<Vector2D>> positions_map;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int block = grid[i][j];
            positions_map[block].push_back(Vector2D(j, i));
        }
    }

    std::vector<MapObject> blocks;

    std::transform(positions_map.begin(), positions_map.end(), std::back_inserter(blocks),
                   [this](const auto& pair) {
                       return MapObject{pair.second, pair.first,
                                        texture_parser.get_texture_info(pair.first).collidable};
                   });


    std::vector<Vector2D> ct_spawns = {Vector2D(1, 1), Vector2D(1, 2), Vector2D(2, 1),
                                       Vector2D(2, 2)};
    std::vector<Vector2D> tt_spawns = {Vector2D(13, 6), Vector2D(14, 6), Vector2D(11, 7),
                                       Vector2D(12, 7)};
    std::vector<Vector2D> sites = {Vector2D(12, 1), Vector2D(12, 2), Vector2D(13, 1),
                                   Vector2D(14, 1)};

    GameMap game_map = {width, height, blocks, ct_spawns, tt_spawns, sites};
    return game_map;
}
