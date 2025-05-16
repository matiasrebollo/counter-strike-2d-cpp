#include "client_sender.h"

ClientSender::ClientSender(ServerProtocol& protocol) : protocol(protocol), queue() {
}

void ClientSender::add_snapshot_to_queue(const Snapshot& snapshot) {
    this->queue.push(snapshot);
}
void ClientSender::send_snapshot_to_client() {
    Snapshot snapshot = this->queue.pop();
    this->protocol.send_snapshot(snapshot);
}
void ClientSender::run() {
    this->keep_running = true;
    while (this->keep_running) {
        send_snapshot_to_client();
        // Im sleeping inside the queue so i not burning CPU (i think)
    }
}

ClientSender::~ClientSender() {
    this->keep_running = false;
}