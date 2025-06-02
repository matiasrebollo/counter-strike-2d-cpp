#include "client_sender.h"

ClientSender::ClientSender(ClientProtocol& protocol): protocol(protocol) {}

void ClientSender::run() {
    try {
        while (this->send_command_to_server()) {}
    } catch (const CommunicationEnded& e) {
        std::cout << MSG_CLOSE_SENDER << std::endl;
        return;
    }
}

void ClientSender::add_command_to_queue(const CommandDTO& dto) { this->queue.push(dto); }

void ClientSender::close_queue() { this->queue.close(); }

bool ClientSender::send_command_to_server() {
    try {
        CommandDTO dto = this->queue.pop();
        protocol.send_command(dto);
        return true;
    } catch (const ClosedQueue& e) {
        std::cout << MSG_CLOSE_SENDER << std::endl;
        this->stop();
        return false;
    }
}
