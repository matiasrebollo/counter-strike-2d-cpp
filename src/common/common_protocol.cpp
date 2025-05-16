#include "common_protocol.h"

#include <utility>

#include <string.h>

#include "communication_ended.h"

CommonProtocol::CommonProtocol(const std::string& hostname, const std::string& port):
        socket(hostname.c_str(), port.c_str()) {}

CommonProtocol::CommonProtocol(Socket&& socket): socket(std::move(socket)) {}

uint8_t CommonProtocol::receive_byte() {
    uint8_t number;

    size_t received = this->socket.recvall(&number, sizeof(number));
    if (received != sizeof(number)) {
        throw CommunicationEnded();
    }

    return number;
}

uint16_t CommonProtocol::receive_big_endian_number() {
    uint16_t number;

    size_t received = this->socket.recvall(&number, sizeof(number));
    if (received != sizeof(number)) {
        throw CommunicationEnded();
    }

    return ntohs(number);
}

void CommonProtocol::send_byte(const uint8_t& number) {
    size_t sended = this->socket.sendall(&number, sizeof(number));
    if (sended != sizeof(number)) {
        throw CommunicationEnded();
    }
}

void CommonProtocol::send_big_endian_number(const uint16_t& number) {
    uint16_t parsed = htons(number);

    size_t sended = this->socket.sendall(&parsed, sizeof(parsed));
    if (sended != sizeof(parsed)) {
        throw CommunicationEnded();
    }
}

void CommonProtocol::send_string(const std::string& s) {
    this->send_big_endian_number(s.size());
    size_t sended = this->socket.sendall(s.c_str(), s.size());
    if (sended != s.size()) {
        throw CommunicationEnded();
    }
}


std::string CommonProtocol::receive_string() {
    size_t lengthString = this->receive_big_endian_number();
    std::vector<uint8_t> vectorBytes(lengthString);
    size_t received = this->socket.recvall(vectorBytes.data(), vectorBytes.size());
    if (received != lengthString) {
        throw CommunicationEnded();
    }
    std::string s(vectorBytes.begin(), vectorBytes.end());
    return s;
}
