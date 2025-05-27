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
    for (size_t i = 0; i < this->clients.size();) {
        ClientHandler* client = this->clients[i];
        if (!client->is_alive()) {
            client->join();
            delete client;
            this->clients.erase(this->clients.begin() + i);
        } else {
            i++;
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
