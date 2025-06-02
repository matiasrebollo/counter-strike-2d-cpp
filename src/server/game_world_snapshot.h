#ifndef GAME_WORLD_SNAPSHOT_H
#define GAME_WORLD_SNAPSHOT_H

#include <vector>

#include "common/player_dto.h"

struct GameWorldSnapshot {
    // BombStatus bomb_status;
    const std::vector<PlayerDTO> ctt;
    const std::vector<PlayerDTO> tt;
    // std::vector<itemDTO> items;
};


#endif
