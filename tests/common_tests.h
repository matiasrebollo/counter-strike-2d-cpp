#ifndef COMMON_TESTS_H
#define COMMON_TESTS_H

#include <string>
#include <utility>

#include "../src/client/client_protocol.h"
#include "../src/server/server_protocol.h"

std::pair<ClientProtocol, ServerProtocol> create_connected_protocols(const std::string& port);

#endif
