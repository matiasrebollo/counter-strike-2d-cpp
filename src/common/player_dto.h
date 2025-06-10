#ifndef PLAYER_DTO_H
#define PLAYER_DTO_H

#include <cstdint>
#include <string>

#include "common/loadout_dto.h"
#include "common/vector_2d.h"
#include "common/weapon_type.h"

struct PlayerDTO {
    std::string username;
    Vector2D<int> position;
    double orientation;
    uint16_t life;
    LoadoutDTO loadout;
};


#endif
