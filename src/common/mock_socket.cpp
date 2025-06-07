#include "mock_socket.h"

#include <algorithm>
#include <cstring>
#include <iostream>

MockSocket::MockSocket(const char* hostname, const char* servname): buffer(), val(false) {
    (void)hostname;
    (void)servname;
}

MockSocket::MockSocket(const char* servname): buffer(), val(false) { (void)servname; }

int MockSocket::sendall(const void* data, unsigned int size) {
    const uint8_t* parsed = static_cast<const uint8_t*>(data);
    this->buffer.insert(this->buffer.end(), parsed, parsed + size);
    return size;
}

int MockSocket::recvall(void* data, unsigned int size) {
    if (this->buffer.empty()) {
        std::cout << "[DEBUG] dice q esta empty el buffer" << std::endl;
        return -1;
    }

    unsigned int to_read = std::min(size, static_cast<unsigned int>(this->buffer.size()));
    std::memcpy(data, this->buffer.data(), to_read);
    this->buffer.erase(this->buffer.begin(), this->buffer.begin() + to_read);
    return to_read;
}

void MockSocket::shutdown(int how) { (void)how; }


int MockSocket::close() { return 0; }

bool MockSocket::is_stream_recv_closed() { return this->val; }

bool MockSocket::is_stream_send_closed() { return this->val; }
