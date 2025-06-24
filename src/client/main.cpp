#include <iostream>

#include <yaml-cpp/yaml.h>

#include "client.h"

#define MESSAGE_ERROR_LOADING_SETTINGS "There is an error in the settings file"

int main(int argc, char* argv[]) {
    try {
        Settings::getInstance();
        Client client;
        client.run(argc, argv);
        return 0;
    } catch (const YAML::Exception& e) {
        std::cerr << MESSAGE_ERROR_LOADING_SETTINGS << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
