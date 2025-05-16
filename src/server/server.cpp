#include "server.h"

Server::Server(const std::string& port): acceptor(port) {}

int Server::Run() {
    try {
        this->acceptor.start();
        while (std::cin.get() != MESSAGE_KILL_SERVER) {}
        this->acceptor.clear();
        this->acceptor.join();
    } catch (const CommunicationEnded& e) {}
    return 0;
}
