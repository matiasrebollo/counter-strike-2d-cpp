#ifndef COMMON_PROTOCOL_PARSER_H
#define COMMON_PROTOCOL_PARSER_H

#include <cstdint>
#include <unordered_map>

#include "commands.h"

#define CODE_CREATE_USERNAME 0x01
#define CODE_CREATE_GAME 0x02
#define CODE_JOIN_GAME 0x03
#define CODE_SELECT_MAP 0x05
#define CODE_BUY_WEAPON 0x06
#define CODE_BUY_BULLETS 0x07
#define CODE_ROTATE 0x08
#define CODE_MOVE 0x09
#define CODE_SHOOT 0x10
#define CODE_CHANGE_WEAPON 0x11
#define CODE_PLANT_BOMB 0x12
#define CODE_DEFUSE_BOMB 0x13
#define CODE_SEND_MAP 0x25
#define CODE_GAME_STARTED 0x30
#define CODE_SNAPSHOT 0x35
#define CODE_PLAYERS 0x40
#define CODE_BULLETS 0x50

class CodesParser {
protected:
    std::unordered_map<uint8_t, CommandType> codeToCommands;
    std::unordered_map<CommandType, uint8_t> commandsToCode;

public:
    CodesParser();
};

#endif
