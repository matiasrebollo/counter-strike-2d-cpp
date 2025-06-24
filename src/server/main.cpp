#include <iostream>

#include <yaml-cpp/yaml.h>

#include "../common/error_codes.h"
#include "../common/liberror.h"

#include "server.h"

#define AMOUNT_OF_EXPECTED_ARGUMENTS 2
#define MESSAGE_BAD_AMOUNT_OF_EXPECTED_ARGUMENTS "You have to enter the port as an argument"
#define MESSAGE_ERROR_LOADING_SETTINGS "There is an error in the settings file"
#define ARG_INDEX_PORT 1

int main(int argc, char* argv[]) {
    if (argc != AMOUNT_OF_EXPECTED_ARGUMENTS) {
        std::cerr << MESSAGE_BAD_AMOUNT_OF_EXPECTED_ARGUMENTS << std::endl;
        return ErrorCodes::INVALID_ARGUMENTS;
    }
    std::string port = std::string(argv[ARG_INDEX_PORT]);
    try {
        Settings::getInstance();
        Server server = Server(port);
        return server.Run();
    } catch (const LibError& e) {
        return ErrorCodes::SOCKET_ERROR;
    } catch (const YAML::Exception& e) {
        std::cerr << MESSAGE_ERROR_LOADING_SETTINGS << std::endl;
    }
}
