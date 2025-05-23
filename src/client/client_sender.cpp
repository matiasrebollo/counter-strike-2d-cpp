#include "client_sender.h"

ClientSender::ClientSender(ClientProtocol& protocol): protocol(protocol) {}

void ClientSender::run() {
    while (this->should_keep_running()) {
        send_command_to_server();
        // Im sleeping inside the queue so im not burning CPU (i think)
    }
}

void ClientSender::add_command_to_queue(const CommandDTO& dto) { this->queue.push(dto); }

void ClientSender::send_command_to_server() {
    CommandDTO dto = this->queue.pop();
    protocol.send_command(dto);
}
