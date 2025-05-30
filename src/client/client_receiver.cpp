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

bool ClientReceiver::try_pop_game_response(GameResponseDTO& response) {
    return this->queue.try_pop(response);
}

void ClientReceiver::close_queue() { this->queue.close(); }

void ClientReceiver::receive_snapshot_from_server() {
    GameResponseDTO response = this->protocol.receive_game_response();
    this->queue.push(response);
}
