#ifndef COMMON_TESTS_H
#define COMMON_TESTS_H

#include <memory>
#include <string>
#include <utility>

#include "../src/client/client_protocol.h"
#include "../src/server/server_protocol.h"
#define SOCKET_PORT "10000"

std::pair<std::unique_ptr<ClientProtocol>, std::unique_ptr<ServerProtocol>>
        create_connected_protocols();


#endif
