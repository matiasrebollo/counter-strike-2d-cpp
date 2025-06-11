#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <unordered_map>
#include <vector>

#include "common/map_object.h"
#include "common/vector_2d.h"

struct GameMap {
    int width;
    int height;
    Background background;
    std::vector<MapObject> map_objects;
    std::vector<Vector2D<int>> ct_spawns;
    std::vector<Vector2D<int>> tt_spawns;
    std::vector<Vector2D<int>> sites;
};

struct GameMapDTO {
    Background background;
    std::vector<MapObject> map_objects;
};

struct ShopInfoDTO {
    std::unordered_map<GunType, int> shop_gun_prices;
    std::unordered_map<GunType, int> shop_clip_by_gun_prices;
};

struct GameInitialInfoDTO {
    GameMapDTO game_map;
    ShopInfoDTO shop_info;
};

#endif
