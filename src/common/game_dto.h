#ifndef GAME_DTO_H
#define GAME_DTO_H

#include <variant>

#include "game_map.h"
#include "game_snapshot.h"

using GameDTO = std::variant<GameMap, Snapshot>;

#endif
