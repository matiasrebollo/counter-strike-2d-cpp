#include "client_sender.h"

ClientSender::ClientSender(ClientProtocol& protocol): protocol(protocol) {}

void ClientSender::run() {
    while (this->should_keep_running()) {
        try {
            send_command_to_server();
            // Im sleeping inside the queue so im not burning CPU (i think)
        } catch (const CommunicationEnded& e) {
            std::cout << MSG_CLOSE_SENDER << std::endl;
            return;
        } catch (const ClosedQueue& e) {
            std::cout << MSG_CLOSE_SENDER << std::endl;
            return;
        }
    }
}

void ClientSender::add_command_to_queue(const CommandDTO& dto) { this->queue.push(dto); }

void ClientSender::close_queue() { this->queue.close(); }

void ClientSender::send_command_to_server() {
    CommandDTO dto = this->queue.pop();
    protocol.send_command(dto);
}
