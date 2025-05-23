#include <iostream>

#include "client.h"

int main(int argc, char* argv[]) {
    try {
        Client client;
        client.run(argc, argv);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
