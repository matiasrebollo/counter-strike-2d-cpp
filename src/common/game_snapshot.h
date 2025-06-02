#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <vector>

#include "common/phase.h"
#include "common/player_dto.h"

struct Snapshot {
    Phase phase;
    int round_number;
    int total_rounds;
    // BombStatus bomb_status;
    int timer;
    std::vector<PlayerDTO> ctt;
    std::vector<PlayerDTO> tt;
    // std::vector<itemDTO> items;
    // eventos de la partida?? como disparos o activaciones de bomba (en el playerDTO o separado?)
};


#endif
