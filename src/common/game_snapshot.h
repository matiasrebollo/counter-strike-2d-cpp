#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <optional>
#include <vector>

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
    // BombStatus bomb_status;
    int time_left;
    std::vector<PlayerDTO> ct;
    std::vector<PlayerDTO> tt;
    // std::optional<Team> current_round_winner;
    //  std::vector<PlayerDTO> players;
    //  std::vector<itemDTO> items;
    //  eventos de la partida?? como disparos o activaciones de bomba (en el playerDTO o separado?)
};


#endif
