#include "client_receiver.h"

ClientReceiver::ClientReceiver(ClientProtocol& protocol): protocol(protocol) {}

void ClientReceiver::run() {
    while (this->should_keep_running()) {
        try {
            receive_snapshot_from_server();
        } catch (const CommunicationEnded& e) {
            std::cout << MSG_CLOSE_RECEIVER << std::endl;
            this->close_queue();
            this->stop();
        } catch (const ClosedQueue& e) {
            std::cout << MSG_CLOSE_RECEIVER << std::endl;
            this->stop();
            return;
        }
    }
}

bool ClientReceiver::try_pop_game_dto(GameDTO& response) { return this->queue.try_pop(response); }

void ClientReceiver::close_queue() {
    try {
        this->queue.close();
    } catch (const ClosedQueue& e) {}
}

void ClientReceiver::receive_snapshot_from_server() {
    GameDTO response = this->protocol.receive_game_dto();
    this->queue.push(response);
}
