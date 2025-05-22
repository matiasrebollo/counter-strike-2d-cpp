#include <string>
#include <vector>

#include <arpa/inet.h>

#include "error_codes.h"
#include "socket.h"


class CommonProtocol {
protected:
    Socket socket;

public:
    CommonProtocol(const std::string& hostname, const std::string& port);
    explicit CommonProtocol(Socket&& socket);

    uint8_t receive_byte();
    uint16_t receive_big_endian_number();
    void send_byte(const uint8_t& number);
    void send_big_endian_number(const uint16_t& number);
    void send_string(const std::string& s);
    std::string receive_string();
};
