#include "common_protocol.h"

#include <iostream>
#include <utility>

#include <string.h>

#include "communication_ended.h"
#include "liberror.h"

CommonProtocol::CommonProtocol(std::unique_ptr<Socket> socket):
        socket(std::move(socket)),
        weaponParser(),
        codeToCommands({{CODE_CREATE_USERNAME, CommandType::CREATE_USERNAME},
                        {CODE_CREATE_GAME, CommandType::CREATE_GAME},
                        {CODE_JOIN_GAME, CommandType::JOIN_GAME},
                        {CODE_SELECT_MAP, CommandType::SELECT_MAP},
                        {CODE_BUY_WEAPON, CommandType::BUY_WEAPON},
                        {CODE_BUY_BULLETS, CommandType::BUY_AMMO},
                        {CODE_ROTATE, CommandType::ROTATE},
                        {CODE_MOVE, CommandType::MOVE},
                        {CODE_CHANGE_WEAPON, CommandType::CHANGE_WEAPON},
                        {CODE_PLANT_BOMB, CommandType::PLANT_BOMB},
                        {CODE_GAME_STARTED, CommandType::GAME_STARTED},
                        {CODE_ENDGAME, CommandType::GAME_ENDED}}),
        commandsToCode({{CommandType::CREATE_USERNAME, CODE_CREATE_USERNAME},
                        {CommandType::CREATE_GAME, CODE_CREATE_GAME},
                        {CommandType::JOIN_GAME, CODE_JOIN_GAME},
                        {CommandType::SELECT_MAP, CODE_SELECT_MAP},
                        {CommandType::BUY_WEAPON, CODE_BUY_WEAPON},
                        {CommandType::BUY_AMMO, CODE_BUY_BULLETS},
                        {CommandType::ROTATE, CODE_ROTATE},
                        {CommandType::MOVE, CODE_MOVE},
                        {CommandType::CHANGE_WEAPON, CODE_CHANGE_WEAPON},
                        {CommandType::PLANT_BOMB, CODE_PLANT_BOMB},
                        {CommandType::GAME_STARTED, CODE_GAME_STARTED},
                        {CommandType::GAME_ENDED, CODE_ENDGAME}}) {}

CommonProtocol::CommonProtocol(CommonProtocol&& other) noexcept:
        socket(std::move(other.socket)),
        weaponParser(std::move(other.weaponParser)),
        codeToCommands(std::move(other.codeToCommands)),
        commandsToCode(std::move(other.commandsToCode)) {}


// cppcheck-suppress operatorEqVarError
CommonProtocol& CommonProtocol::operator=(CommonProtocol&& other) noexcept {
    if (this != &other) {
        socket = std::move(other.socket);
        weaponParser = std::move(other.weaponParser);
        codeToCommands = std::move(other.codeToCommands);
        commandsToCode = std::move(other.commandsToCode);
    }
    return *this;
}

uint8_t CommonProtocol::receive_byte() {
    uint8_t number;

    try {
        size_t received = this->socket->recvall(&number, sizeof(number));
        if (received != sizeof(number)) {
            throw CommunicationEnded();
        }
    } catch (const LibError& e) {
        throw CommunicationEnded();
    }

    return number;
}

uint16_t CommonProtocol::receive_big_endian_number() {
    uint16_t number;

    try {
        size_t received = this->socket->recvall(&number, sizeof(number));
        if (received != sizeof(number)) {
            throw CommunicationEnded();
        }
    } catch (const LibError& e) {
        throw CommunicationEnded();
    }

    return ntohs(number);
}

void CommonProtocol::send_byte(const uint8_t& number) {
    try {
        size_t sended = this->socket->sendall(&number, sizeof(number));
        if (sended != sizeof(number)) {
            throw CommunicationEnded();
        }
    } catch (const LibError& e) {
        throw CommunicationEnded();
    }
}

void CommonProtocol::send_big_endian_number(const uint16_t& number) {
    uint16_t parsed = htons(number);

    try {
        size_t sended = this->socket->sendall(&parsed, sizeof(parsed));
        if (sended != sizeof(parsed)) {
            throw CommunicationEnded();
        }
    } catch (const LibError& e) {
        throw CommunicationEnded();
    }
}

void CommonProtocol::send_string(const std::string& s) {
    this->send_big_endian_number(s.size());
    try {
        size_t sended = this->socket->sendall(s.c_str(), s.size());
        if (sended != s.size()) {
            throw CommunicationEnded();
        }
    } catch (const LibError& e) {
        throw CommunicationEnded();
    }
}


std::string CommonProtocol::receive_string() {
    size_t lengthString = this->receive_big_endian_number();
    std::vector<uint8_t> vectorBytes(lengthString);
    try {
        size_t received = this->socket->recvall(vectorBytes.data(), vectorBytes.size());
        if (received != lengthString) {
            throw CommunicationEnded();
        }
        std::string s(vectorBytes.begin(), vectorBytes.end());
        return s;
    } catch (const LibError& e) {
        throw CommunicationEnded();
    }
}

void CommonProtocol::send_angle(const double& angle) {
    uint16_t encoded = static_cast<uint16_t>((angle / 360.0f) * 65535.0f);
    this->send_big_endian_number(encoded);
}

double CommonProtocol::receive_angle() {
    uint16_t encoded = this->receive_big_endian_number();

    return (static_cast<double>(encoded) / 65535.0f) * 360.0f;
}
