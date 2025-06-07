#pragma once

#ifndef MOCK_SOCKET_H
#define MOCK_SOCKET_H

#include <cstdint>
#include <vector>

class MockSocket {
private:
    std::vector<uint8_t> buffer;
    bool val;

public:
    MockSocket(const char* hostname, const char* servname);
    explicit MockSocket(const char* servname);
    int sendall(const void* data, unsigned int size);
    int recvall(void* data, unsigned int size);
    void shutdown(int how);
    int close();
    bool is_stream_recv_closed();
    bool is_stream_send_closed();
    MockSocket(const MockSocket&) = delete;
    MockSocket& operator=(const MockSocket&) = delete;
};


#endif
