#include "client_receiver.h"

ClientReceiver::ClientReceiver(ClientProtocol& protocol): protocol(protocol) {}

void ClientReceiver::run() {
    while (this->should_keep_running()) {
        try {
            receive_snapshot_from_server();
            // Im sleeping inside the queue so im not burning CPU (i think)
        } catch (const CommunicationEnded& e) {
            std::cout << MSG_CLOSE_RECEIVER << std::endl;
            return;
        } catch (const ClosedQueue& e) {
            std::cout << MSG_CLOSE_RECEIVER << std::endl;
            return;
        }
    }
}

Snapshot ClientReceiver::pop_snapshot_from_queue() { return this->queue.pop(); }

GameMap ClientReceiver::receive_initial_map() { return this->protocol.receive_map(); }

void ClientReceiver::close_queue() { this->queue.close(); }

void ClientReceiver::receive_snapshot_from_server() {
    Snapshot snapshot = this->protocol.receive_snapshot();
    this->queue.push(snapshot);
}
