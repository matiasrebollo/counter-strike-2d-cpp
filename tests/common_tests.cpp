#include "common_tests.h"

#include <chrono>
#include <thread>

#include "../src/common/socket.h"

std::pair<ClientProtocol, ServerProtocol> create_connected_protocols(const std::string& port) {
    Socket listener(port.c_str());
    ClientProtocol client("localhost", port);
    Socket accepted_skt = listener.accept();
    ServerProtocol server(std::move(accepted_skt));
    return {std::move(client), std::move(server)};
}
