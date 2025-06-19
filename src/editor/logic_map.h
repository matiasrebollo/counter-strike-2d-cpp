#ifndef LOGIC_MAP_H
#define LOGIC_MAP_H

#include <map>
#include <set>
#include <utility>
#include <vector>

#include "../common/block_texture_parser.h"
#include "../common/game_map.h"
#include "../common/weapon_type.h"

struct CellInfo {
    const int block;
    bool tt_spawn;
    bool ct_spawn;
    bool bomb_site;
    GunType gun;
};

class LogicMap {
public:
    LogicMap();

    void loadMap(const GameMap& map);
    const GameMap createMap();

    Background getBackground();
    void clearMap();
    void clearGrid();
    int getWidth();
    int getHeight();
    const CellInfo getCellInfo(const int& row, const int& column);

    void addColumns();
    void addRows();

    void setBlock(const int& row, const int& column, const bool& to_delete);
    void setCtSpawn(const int& row, const int& column, const bool& to_delete);
    void setTTSpawn(const int& row, const int& column, const bool& to_delete);
    void setBombSite(const int& row, const int& column, const bool& to_delete);
    void setGun(const int& row, const int& column, const bool& to_delete);
    void selectBackground(const Background& background);
    void selectBlock(const int& block);
    void selectGun(const GunType& gun);

private:
    BlockTextureParser texture_parser;
    std::vector<std::vector<int>> grid;
    int selected_block;
    Background selected_background;
    GunType selected_gun;
    std::set<std::pair<int, int>> tt_spawns;
    std::set<std::pair<int, int>> ct_spawns;
    std::set<std::pair<int, int>> bomb_sites;
    std::map<std::pair<int, int>, GunType> guns;

    std::vector<MapObject> load_blocks(const int& offset_x, const int& offset_y);
    std::vector<Vector2D<int>> set_to_vector(const std::set<std::pair<int, int>>& set_pos,
                                             const int& offset_x, const int& offset_y);
    std::map<GunType, std::vector<Vector2D<int>>> save_guns(const int& offset_x,
                                                            const int& offset_y);
};

#endif
