#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <optional>
#include <vector>

#include "common/phase.h"
#include "common/player_dto.h"

enum Team { TT, CT };  // para el protocolo se puede crear otro enum y reemplazar este por ese

struct Snapshot {
    Phase phase;
    int current_round_number;
    int total_rounds;
    // BombStatus bomb_status;
    int time_left;
    std::vector<PlayerDTO> ct;
    std::vector<PlayerDTO> tt;
    std::optional<Team> current_round_winner;
    // std::vector<PlayerDTO> players;
    // std::vector<itemDTO> items;
    // eventos de la partida?? como disparos o activaciones de bomba (en el playerDTO o separado?)
};


#endif
