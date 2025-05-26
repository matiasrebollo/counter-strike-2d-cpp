#include "client_sender.h"

#include <memory>

ClientSender::ClientSender(ServerProtocol& protocol, std::string& username):
        queue(), protocol(protocol), keep_running(true), username(username) {}

void ClientSender::push(const Snapshot& snapshot) {
    try {
        this->queue.try_push(snapshot);
    } catch (const ClosedQueue& e) {
        std::cout << "Intente pushear a queue cerrada" << std::endl;
    }
}

void ClientSender::send_snapshot() {
    Snapshot snapshot = this->queue.pop();
    this->protocol.send_snapshot(snapshot);
}

void ClientSender::send_map(const GameMap& map) { this->protocol.send_map(map); }

void ClientSender::run() {
    while (this->keep_running) {
        try {
            this->send_snapshot();
        } catch (const ClosedQueue& e) {
            std::cout << "Intente popear de queue cerrada" << std::endl;
            break;
        }
    }
    this->queue.close();
}

ClientSender::~ClientSender() {}
