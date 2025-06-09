#include "common_tests.h"

#include <vector>

#include "../src/common/mock_socket.h"
using Socket = MockSocket;

std::pair<std::unique_ptr<ClientProtocol>, std::unique_ptr<ServerProtocol>>
        create_connected_protocols() {

    auto client_to_server = std::make_shared<std::vector<uint8_t>>();
    auto server_to_client = std::make_shared<std::vector<uint8_t>>();

    auto client_socket = std::make_unique<MockSocket>(client_to_server, server_to_client);
    auto server_socket = std::make_unique<MockSocket>(server_to_client, client_to_server);

    auto client = std::make_unique<ClientProtocol>(std::move(client_socket));
    auto server = std::make_unique<ServerProtocol>(std::move(server_socket));

    return {std::move(client), std::move(server)};
}
