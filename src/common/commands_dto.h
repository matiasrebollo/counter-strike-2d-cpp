#ifndef COMMANDS_DTO_H
#define COMMANDS_DTO_H

#include <variant>

struct MoveUpDTO {};
struct MoveDownDTO {};
struct MoveLeftDTO {};
struct MoveRightDTO {};

struct RotateDTO {
    const float angle;
};

using CommandDTO = std::variant<MoveUpDTO, MoveDownDTO, MoveLeftDTO, MoveRightDTO, RotateDTO>;

#endif
