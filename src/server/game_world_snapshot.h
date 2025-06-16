#ifndef GAME_WORLD_SNAPSHOT_H
#define GAME_WORLD_SNAPSHOT_H

#include <vector>

#include "common/player_dto.h"

struct GameWorldSnapshot {
    BombStatus bomb_status;
    std::optional<Vector2D<int>> bomb_position;
    const std::vector<PlayerDTO> ct;
    const std::vector<PlayerDTO> tt;
    // std::vector<itemDTO> items;
};


#endif
