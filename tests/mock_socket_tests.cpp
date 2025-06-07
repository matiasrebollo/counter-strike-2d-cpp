#include <gtest/gtest.h>

#include "../src/common/mock_socket.h"

TEST(MockSocketTest, SendBytes) {
    MockSocket socket("testiing");

    std::vector<uint8_t> data = {0x01, 0x02, 0x03};
    socket.sendall(data.data(), data.size());

    std::vector<uint8_t> expected = {0x01, 0x02, 0x03};
    uint8_t read[3];
    int received = socket.recvall(read, 3);

    EXPECT_EQ(received, 3);
    EXPECT_EQ(std::vector<uint8_t>(read, read + 3), expected);
}
