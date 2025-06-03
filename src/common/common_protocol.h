#pragma once

#include <numbers>
#include <string>
#include <vector>

#include <arpa/inet.h>

#include "error_codes.h"
#include "socket.h"

template <class>
inline constexpr bool always_false_v = false;

class CommonProtocol {
protected:
    Socket socket;
    WeaponParser weaponParser;

public:
    CommonProtocol(const std::string& hostname, const std::string& port);
    explicit CommonProtocol(Socket&& socket);

    double receive_angle();
    void send_angle(const double& angle);
    uint8_t receive_byte();
    uint16_t receive_big_endian_number();
    void send_byte(const uint8_t& number);
    void send_big_endian_number(const uint16_t& number);
    void send_string(const std::string& s);
    std::string receive_string();
};
