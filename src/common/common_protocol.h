#pragma once

#include <cstdint>
#include <memory>
#include <numbers>
#include <string>
#include <unordered_map>
#include <vector>

#include <arpa/inet.h>

#include "commands.h"
#include "commands_dto.h"
#include "error_codes.h"
#include "weapon_parser.h"

#ifdef TESTS
#include "../common/mock_socket.h"
using Socket = MockSocket;
#else
#include "../common/socket.h"
using Socket = RealSocket;
#endif

template <class>
inline constexpr bool always_false_v = false;

#define CODE_TRUE 0x01
#define CODE_FALSE 0x00

#define CODE_CREATE_USERNAME 0x01
#define CODE_CREATE_GAME 0x02
#define CODE_JOIN_GAME 0x03
#define CODE_SELECT_MAP 0x05
#define CODE_BUY_WEAPON 0x06
#define CODE_BUY_BULLETS 0x07
#define CODE_ROTATE 0x08
#define CODE_MOVE 0x09
#define CODE_ACTION 0x10
#define CODE_CHANGE_WEAPON 0x11
#define CODE_PLANT_BOMB 0x12
#define CODE_SEND_GAME_INIT_INFO 0x25
#define CODE_GAME_STARTED 0x30
#define CODE_SNAPSHOT 0x35
#define CODE_PLAYERS 0x40
#define CODE_BULLETS 0x50
#define CODE_SHOT 0x55
#define CODE_ENDGAME 0x60

#define CODE_CHOOSE_KNIFE 0x00
#define CODE_CHOOSE_PRIMARY 0x01
#define CODE_CHOOSE_SECONDARY 0x02
#define CODE_CHOOSE_BOMB 0x03

class CommonProtocol {
protected:
    std::unique_ptr<Socket> socket;
    WeaponParser weaponParser;
    std::unordered_map<uint8_t, CommandType> codeToCommands;
    std::unordered_map<CommandType, uint8_t> commandsToCode;
    std::unordered_map<bool, uint8_t> bools_to_code;
    std::unordered_map<uint8_t, bool> code_to_bools;

public:
    explicit CommonProtocol(std::unique_ptr<Socket> socket);

    CommonProtocol(CommonProtocol&& other) noexcept;
    CommonProtocol& operator=(CommonProtocol&& other) noexcept;

    double receive_angle();
    void send_angle(const double& angle);
    uint8_t receive_byte();
    uint16_t receive_big_endian_number();
    void send_byte(const uint8_t& number);
    void send_big_endian_number(const uint16_t& number);
    void send_string(const std::string& s);
    std::string receive_string();
};
