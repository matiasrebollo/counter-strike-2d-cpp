#pragma once

#include <cstdint>
#include <memory>
#include <numbers>
#include <string>
#include <unordered_map>
#include <vector>

#include <arpa/inet.h>

#include "commands.h"
#include "error_codes.h"
#include "game_commands_dto.h"
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
#define CODE_DEFUSE_BOMB 0x13
#define CODE_PICK_UP 0x14
#define CODE_START 0x15
#define CODE_SEND_GAME_INIT_INFO 0x25
#define CODE_GAME_STARTED 0x30
#define CODE_SNAPSHOT 0x35
#define CODE_PLAYERS 0x40
#define CODE_BULLETS 0x50
#define CODE_SHOT 0x55
#define CODE_ENDGAME 0x60
#define CODE_GUN_DROPPED 0x61
#define CODE_BOMB_DROPPED 0x62

#define CODE_CHOOSE_KNIFE 0x00
#define CODE_CHOOSE_PRIMARY 0x01
#define CODE_CHOOSE_SECONDARY 0x02
#define CODE_CHOOSE_BOMB 0x03

#define CODE_NEGATIVE_NUMBER 0x01
#define CODE_POSITIVE_NUMBER 0x00

/*
    Class from who server and client protocol inherite. It provides the interface for the
    send and recv "low level" methods.
*/
class CommonProtocol {
protected:
    std::unique_ptr<Socket> socket;
    WeaponParser weaponParser;
    std::unordered_map<uint8_t, CommandType> codeToCommands;
    std::unordered_map<CommandType, uint8_t> commandsToCode;
    std::unordered_map<bool, uint8_t> bools_to_code;
    std::unordered_map<uint8_t, bool> code_to_bools;

    double receive_angle();
    void send_angle(const double& angle);

    uint8_t receive_byte();
    void send_byte(const uint8_t& number);
    /*
        Receive and send a 2 bytes number in big endian.
    */
    uint16_t receive_big_endian_number();
    void send_big_endian_number(const uint16_t& number);

    void send_string(const std::string& s);
    std::string receive_string();

    void send_double(const double& number);
    double receive_double();

public:
    explicit CommonProtocol(std::unique_ptr<Socket> socket);

    CommonProtocol(CommonProtocol&& other) noexcept;
    CommonProtocol& operator=(CommonProtocol&& other) noexcept;
};
