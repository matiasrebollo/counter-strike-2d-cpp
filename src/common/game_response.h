#ifndef GAME_RESPONSE_H
#define GAME_RESPONSE_H

#include <variant>

#include "game_map.h"
#include "game_snapshot.h"

using GameResponseDTO = std::variant<GameMap, Snapshot>;

#endif
