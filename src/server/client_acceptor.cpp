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
            this->stop();
        } catch (std::exception&) {
            this->stop();
        }
    }
    this->clear();
}

void ClientAcceptor::reap() {
    for (size_t i = 0; i < this->clients.size();) {
        ClientHandler* client = this->clients[i];
        if (!client->is_alive()) {
            // client->kill();
            client->join();
            delete client;
            this->clients.erase(this->clients.begin() + i);
            std::cout << MSG_CLIENT_JOINED(client->get_username()) << std::endl;
        } else {
            i++;
        }
    }
    this->server_monitor.reap_games();
}

void ClientAcceptor::clear() {
    for (auto* client: this->clients) {
        client->kill();
        client->join();
        delete client;
        std::cout << MSG_CLIENT_JOINED(client->get_username()) << std::endl;
    }
    this->clients.clear();
    this->server_monitor.kill_games();
}

void ClientAcceptor::close_acceptor() {
    this->acceptor.shutdown(SHUT_RDWR);
    this->acceptor.close();
}
