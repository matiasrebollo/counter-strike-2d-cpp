#include "client_receiver.h"

ClientReceiver::ClientReceiver(ClientProtocol& protocol): protocol(protocol) {}

void ClientReceiver::run() {
    this->_keep_running = true;
    while (this->should_keep_running()) {
        receive_snapshot_from_server();
        // Im sleeping inside the queue so im not burning CPU (i think)
    }
}

Snapshot ClientReceiver::pop_snapshot_from_queue() { return this->queue.pop(); }

void ClientReceiver::receive_snapshot_from_server() {
    Snapshot snapshot = this->protocol.Receive_snapshot();
    // salta error porque la tengo comentada en el protocolo
    this->queue.push(snapshot);
}

ClientReceiver::~ClientReceiver() {
    this->stop();
    this->join();
}
