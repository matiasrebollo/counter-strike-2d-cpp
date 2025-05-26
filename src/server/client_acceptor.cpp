#include "client_acceptor.h"

#include <string>
#include <utility>

ClientAcceptor::ClientAcceptor(const std::string& port): acceptor(port.c_str()), server_monitor() {}

void ClientAcceptor::run() {
    while ((this->should_keep_running())) {
        try {
            Socket socket = this->acceptor.accept();
            ClientHandler* client = new ClientHandler(std::move(socket), this->server_monitor);
            this->reap();
            this->clients.push_back(client);
            client->start();
        } catch (const LibError& e) {
            break;
        } catch (std::exception&) {
            break;
        }
    }
}

void ClientAcceptor::reap() {
    for (auto* client: this->clients) {
        if (!client->is_alive()) {
            client->join();
            delete client;
        }
    }
}

void ClientAcceptor::clear() {
    for (auto* client: this->clients) {
        client->join();
        delete client;
    }
    this->clients.clear();
    this->stop();
    this->acceptor.shutdown(SHUT_RDWR);
    this->acceptor.close();
}
