#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "phase.h"
#include "bomb_status.h"

struct Snapshot {
    Phase phase;
    int round_number;
    BombStatus bomb_status;
    int timer;
    // std::vector<Player>
    // std::vector<Bullet>
};


#endif
