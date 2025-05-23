#include "client_sender.h"

#include <memory>

ClientSender::ClientSender(ServerProtocol& protocol):
        queue(), protocol(protocol), keep_running(true) {}

void ClientSender::push(const Snapshot& snapshot) { this->queue.push(snapshot); }

void ClientSender::send_snapshot() {
    Snapshot snapshot = this->queue.pop();
    this->protocol.send_snapshot(snapshot);
}

void ClientSender::send_map(const GameMap& map) { this->protocol.send_map(map); }

void ClientSender::run() {
    while (this->keep_running) {
        send_snapshot();
    }
}
