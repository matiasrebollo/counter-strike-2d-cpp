#pragma once

#ifndef MOCK_SOCKET_H
#define MOCK_SOCKET_H

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

class MockSocket {
private:
    std::shared_ptr<std::vector<uint8_t>> input_buffer;
    std::shared_ptr<std::vector<uint8_t>> output_buffer;
    bool val;

public:
    MockSocket(std::shared_ptr<std::vector<uint8_t>> input_buffer,
               std::shared_ptr<std::vector<uint8_t>> output_buffer);

    MockSocket(const char* hostname, const char* servname);
    explicit MockSocket(const char* servname);

    int sendall(const void* data, unsigned int size);
    int recvall(void* data, unsigned int size);

    void shutdown(int how);
    int close();
    bool is_stream_recv_closed();
    bool is_stream_send_closed();

    static std::pair<std::unique_ptr<MockSocket>, std::unique_ptr<MockSocket>> create_pair();

    MockSocket(const MockSocket&) = delete;
    MockSocket& operator=(const MockSocket&) = delete;
};

#endif
