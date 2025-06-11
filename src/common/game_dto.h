#ifndef GAME_DTO_H
#define GAME_DTO_H

#include <variant>

#include "game_ended.h"
#include "game_info_dto.h"
#include "game_snapshot.h"

using GameDTO = std::variant<GameInitialInfoDTO, Snapshot, GameEnded>;

#endif
