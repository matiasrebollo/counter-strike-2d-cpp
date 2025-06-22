#include "client_sender.h"

ClientSender::ClientSender(ClientProtocol& protocol): protocol(protocol) {}

void ClientSender::run() {
    try {
        while (this->should_keep_running()) {
            this->send_command_to_server();
        }
    } catch (const CommunicationEnded& e) {
        std::cout << MSG_CLOSE_SENDER << std::endl;
        this->stop();
        this->close_queue();
        return;
    } catch (const ClosedQueue& e) {
        std::cout << MSG_CLOSE_SENDER << std::endl;
        this->stop();
        return;
    }
}

void ClientSender::add_command_to_queue(const GameCommandDTO& dto) { this->queue.push(dto); }

void ClientSender::close_queue() { this->queue.close(); }

void ClientSender::send_command_to_server() {
    GameCommandDTO dto = this->queue.pop();
    this->protocol.send_command(dto);
}
