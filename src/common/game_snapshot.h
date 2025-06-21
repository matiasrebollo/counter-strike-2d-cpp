#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <optional>
#include <vector>

#include "bomb_status.h"
#include "item_dto.h"
#include "phase.h"
#include "player_dto.h"

enum Team {
    CT = 0,
    TT = 1
};  // para el protocolo se puede crear otro enum y reemplazar este por ese

struct Snapshot {
    int total_players;
    Phase phase;
    size_t current_round_number;
    size_t total_rounds;
    int time_left;
    BombStatus bomb_status;
    std::optional<Vector2D<int>> bomb_position;
    std::vector<PlayerDTO> ct;
    std::vector<PlayerDTO> tt;
    std::optional<Team> current_round_winner;
    std::vector<ItemDTO> items;
};


#endif
