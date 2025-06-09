#include <gtest/gtest.h>

#include "../src/common/mock_socket.h"

TEST(MockSocketTest, SendBytes) {
    auto client_to_server = std::make_shared<std::vector<uint8_t>>();
    auto server_to_client = std::make_shared<std::vector<uint8_t>>();

    auto client_socket = std::make_unique<MockSocket>(client_to_server, server_to_client);
    auto server_socket = std::make_unique<MockSocket>(server_to_client, client_to_server);

    std::vector<uint8_t> data = {0x01, 0x02, 0x03};
    client_socket->sendall(data.data(), data.size());

    std::vector<uint8_t> expected = {0x01, 0x02, 0x03};
    uint8_t read[3];
    int received = server_socket->recvall(read, 3);

    EXPECT_EQ(received, 3);
    EXPECT_EQ(std::vector<uint8_t>(read, read + 3), expected);
}
