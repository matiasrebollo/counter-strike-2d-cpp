#include "common_tests.h"

#include <chrono>
#include <thread>

#include "../src/common/socket.h"

std::pair<std::unique_ptr<ClientProtocol>, std::unique_ptr<ServerProtocol>>
        create_connected_protocols(const std::string& port) {
    Socket listener(port.c_str());
    auto client = std::make_unique<ClientProtocol>("localhost", port);
    Socket accepted_skt = listener.accept();
    auto server = std::make_unique<ServerProtocol>(std::move(accepted_skt));
    return {std::move(client), std::move(server)};
}
