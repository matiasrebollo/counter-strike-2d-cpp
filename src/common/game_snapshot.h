#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <vector>

#include "common/player_dto.h"

struct Snapshot {
    // const Phase phase;
    // const int round_number;
    // const BombStatus bomb_status;
    // const int timer;
    std::vector<PlayerDTO> players;
    // const std::vector<const itemDTO> items;
};


#endif
