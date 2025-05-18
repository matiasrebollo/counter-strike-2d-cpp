#include "client_sender.h"

#include <memory>

ClientSender::ClientSender(std::shared_ptr<Queue<Snapshot>> queue, ServerProtocol& protocol):
        queue(queue), protocol(protocol), keep_running(true) {
    this->protocol.send_map();
}

void ClientSender::add_snapshot_to_queue(const Snapshot& snapshot) { this->queue->push(snapshot); }

void ClientSender::send_snapshot_to_client() {
    Snapshot snapshot = this->queue->pop();
    this->protocol.send_snapshot(snapshot);
}

void ClientSender::run() {
    this->protocol.send_map();
    this->keep_running = true;
    while (this->keep_running) {
        send_snapshot_to_client();
        // Im sleeping inside the queue so i not burning CPU (i think)
    }
}
