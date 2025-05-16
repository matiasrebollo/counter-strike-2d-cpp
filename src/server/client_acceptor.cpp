#include "client_acceptor.h"

#include <string>
#include <utility>

ClientAcceptor::ClientAcceptor(const std::string& port): acceptor(port.c_str()), server_monitor() {}

void ClientAcceptor::run() {
    while (!(this->acceptor.is_stream_recv_closed() && this->acceptor.is_stream_send_closed())) {
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
        client->kill();
        client->join();
        delete client;
    }
    this->clients.clear();
    this->_is_alive = this->_keep_running = false;
    this->acceptor.shutdown(SHUT_RDWR);
    this->acceptor.close();
}
