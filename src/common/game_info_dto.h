#ifndef GAME_INFO_DTO_H
#define GAME_INFO_DTO_H

#include <unordered_map>
#include <vector>

#include "common/map_object.h"
#include "common/weapon_type.h"

struct GameMapDTO {
    Background background;
    std::vector<MapObject> map_objects;
    std::vector<Vector2D<int>> sites;
};

struct ShopInfoDTO {
    std::unordered_map<GunType, int> prices;
    std::unordered_map<GunType, int> ammo_by_clip;
    int price_clips;
};

struct GameInitialInfoDTO {
    GameMapDTO game_map;
    ShopInfoDTO shop_info;
};

#endif
