#define COLUMNS_TO_ADD 10
#define ROWS_TO_ADD 10

#include "logic_map.h"

#include <QMessageBox>
#include <algorithm>
#include <stdexcept>

#define DEFAULT_ROWS 10
#define DEFAULT_COLUMNS 12
#define MIN_SIZE_SPAWNS 10
#define MIN_SIZE_SITES 1
#define TITLE_MSG_ERROR_SPAWNS "No hay suficientes spawns"
#define TITLE_MSG_ERROR_SITES "No hay suficientes sites"
#define MSG_SIZE_CT_SPAWNS \
    "Debes tener por lo menos " + std::to_string(MIN_SIZE_SPAWNS) + " spawns de CT"
#define MSG_SIZE_TT_SPAWNS \
    "Debes tener por lo menos " + std::to_string(MIN_SIZE_SPAWNS) + " spawns de TT"
#define MSG_SIZE_SITES \
    "Debes tener por lo menos " + std::to_string(MIN_SIZE_SITES) + " sites para plantar la bomba"

LogicMap::LogicMap():
        texture_parser(),
        selected_block(NONE_BLOCK),
        selected_background(AZTEC_BACKGROUND),
        selected_gun(NONE) {}

void LogicMap::load_map(const GameMap& map) {
    this->grid.resize(map.height);

    for (auto& row: grid) {
        row.resize(map.width, NONE_BLOCK);
    }
    this->selected_background = map.background;

    for (const auto& object: map.map_objects) {
        for (auto vector: object.positions) {
            this->grid[vector.y][vector.x] = object.type;
        }
    }

    this->tt_spawns.clear();
    this->ct_spawns.clear();
    this->bomb_sites.clear();
    this->guns.clear();

    for (auto vector: map.ct_spawns) {
        this->ct_spawns.emplace(std::make_pair(vector.x, vector.y));
    }
    for (auto vector: map.tt_spawns) {
        this->tt_spawns.emplace(std::make_pair(vector.x, vector.y));
    }
    for (auto vector: map.sites) {
        this->bomb_sites.emplace(std::make_pair(vector.x, vector.y));
    }
    for (const auto& gun: map.guns) {
        for (const auto& vector: gun.second) {
            this->guns[{vector.x, vector.y}] = gun.first;
        }
    }
}

Background LogicMap::get_background() { return this->selected_background; }

void LogicMap::select_background(const Background& background) {
    this->selected_background = background;
}

int LogicMap::get_width() { return static_cast<int>(this->grid[0].size()); }

int LogicMap::get_height() { return static_cast<int>(this->grid.size()); }

void LogicMap::clear_map() {
    this->clear_grid();
    this->tt_spawns.clear();
    this->ct_spawns.clear();
    this->bomb_sites.clear();
    this->selected_background = AZTEC_BACKGROUND;
}

void LogicMap::clear_grid() {
    this->grid = {};
    for (int i = 0; i < DEFAULT_ROWS; i++) {
        this->grid.push_back({});
        for (int j = 0; j < DEFAULT_COLUMNS; j++) {
            this->grid[i].push_back(NONE_BLOCK);
        }
    }
}

const CellInfo LogicMap::get_cell_info(const int& row, const int& column) {
    GunType gun = NONE;
    if (this->guns.find({column, row}) != guns.end()) {
        gun = guns[{column, row}];
    }

    return {this->grid[row][column], this->tt_spawns.find({column, row}) != tt_spawns.end(),
            this->ct_spawns.find({column, row}) != ct_spawns.end(),
            this->bomb_sites.find({column, row}) != bomb_sites.end(), gun};
}

void LogicMap::select_block(const int& block) { this->selected_block = block; }

void LogicMap::select_gun(const GunType& gun) { this->selected_gun = gun; }

const GameMap LogicMap::create_map() {
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
    std::map<GunType, std::vector<Vector2D<int>>> guns_map = save_guns(offset_x, offset_y);

    std::vector<Vector2D<int>> ct_spawns_vector = set_to_vector(ct_spawns, offset_x, offset_y);
    if (ct_spawns_vector.size() < MIN_SIZE_SPAWNS) {
        QMessageBox::information(nullptr, TITLE_MSG_ERROR_SPAWNS,
                                 QString::fromStdString(MSG_SIZE_CT_SPAWNS));
        throw std::runtime_error(MSG_SIZE_CT_SPAWNS);
    }
    std::vector<Vector2D<int>> tt_spawns_vector = set_to_vector(tt_spawns, offset_x, offset_x);
    if (tt_spawns_vector.size() < MIN_SIZE_SPAWNS) {
        QMessageBox::information(nullptr, TITLE_MSG_ERROR_SPAWNS,
                                 QString::fromStdString(MSG_SIZE_TT_SPAWNS));
        throw std::runtime_error(MSG_SIZE_TT_SPAWNS);
    }

    std::vector<Vector2D<int>> sites_vector = set_to_vector(bomb_sites, offset_x, offset_y);
    std::set<Vector2D<int>> sites(sites_vector.begin(), sites_vector.end());
    if (bomb_sites.size() < MIN_SIZE_SITES) {
        QMessageBox::information(nullptr, TITLE_MSG_ERROR_SITES,
                                 QString::fromStdString(MSG_SIZE_SITES));
        throw std::runtime_error(MSG_SIZE_SITES);
    }

    return {right_most - offset_x + 1,
            bottom_most - offset_y + 1,
            selected_background,
            blocks,
            ct_spawns_vector,
            tt_spawns_vector,
            sites,
            guns_map};
}

std::vector<Vector2D<int>> LogicMap::set_to_vector(const std::set<std::pair<int, int>>& set_pos,
                                                   const int& offset_x, const int& offset_y) {
    std::vector<Vector2D<int>> vec;
    vec.reserve(set_pos.size());

    std::transform(set_pos.begin(), set_pos.end(), std::back_inserter(vec),
                   [offset_x, offset_y](const auto& pair) {
                       return Vector2D<int>(pair.first - offset_x, pair.second - offset_y);
                   });
    return vec;
}

std::map<GunType, std::vector<Vector2D<int>>> LogicMap::save_guns(const int& offset_x,
                                                                  const int& offset_y) {
    std::map<GunType, std::vector<Vector2D<int>>> positions_guns;
    for (const auto& gun: this->guns) {
        auto pos = gun.first;
        GunType type = gun.second;
        positions_guns[type].push_back(Vector2D<int>(pos.first - offset_x, pos.second - offset_y));
    }
    return positions_guns;
}

std::vector<MapObject> LogicMap::load_blocks(const int& offset_x, const int& offset_y) {
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

void LogicMap::setBlock(const int& row, const int& column, const bool& to_delete) {
    int block = to_delete ? NONE_BLOCK : selected_block;
    this->grid[row][column] = block;
    if (texture_parser.get_texture_info(block).collidable) {
        setCtSpawn(row, column, true);
        setTTSpawn(row, column, true);
        setBombSite(row, column, true);
        setGun(row, column, true);
    }
}

void LogicMap::setCtSpawn(const int& row, const int& column, const bool& to_delete) {
    if (to_delete) {
        ct_spawns.erase({column, row});
    } else {
        if (texture_parser.get_texture_info(grid[row][column]).collidable) {
            return;
        }
        ct_spawns.insert({column, row});
    }
}

void LogicMap::setTTSpawn(const int& row, const int& column, const bool& to_delete) {
    if (to_delete) {
        tt_spawns.erase({column, row});
    } else {
        if (texture_parser.get_texture_info(grid[row][column]).collidable) {
            return;
        }
        tt_spawns.insert({column, row});
    }
}

void LogicMap::setBombSite(const int& row, const int& column, const bool& to_delete) {
    if (to_delete) {
        bomb_sites.erase({column, row});
    } else {
        if (texture_parser.get_texture_info(grid[row][column]).collidable) {
            return;
        }
        bomb_sites.insert({column, row});
    }
}

void LogicMap::setGun(const int& row, const int& column, const bool& to_delete) {
    if (to_delete) {
        guns.erase({column, row});
    } else {
        if (texture_parser.get_texture_info(grid[row][column]).collidable) {
            return;
        }
        guns[std::make_pair(column, row)] = selected_gun;
    }
}

void LogicMap::add_columns() {
    for (int i = 0; i < this->get_height(); i++) {
        this->grid[i].resize(this->get_width() + COLUMNS_TO_ADD, NONE_BLOCK);
    }
}

void LogicMap::add_rows() {
    this->grid.resize(this->get_height() + ROWS_TO_ADD,
                      std::vector<int>(this->get_width(), NONE_BLOCK));
}
