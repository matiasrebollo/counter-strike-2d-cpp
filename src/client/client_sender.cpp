#include "client_sender.h"

ClientSender::ClientSender(ClientProtocol& protocol): protocol(protocol) {}

void ClientSender::run() {
    while (this->should_keep_running()) {
        send_command_to_server();
        // Im sleeping inside the queue so im not burning CPU (i think)
    }
}

void ClientSender::add_command_to_queue(const MessageFromClient& msg) { this->queue.push(msg); }

void ClientSender::send_command_to_server() {
    MessageFromClient msg = this->queue.pop();
    protocol.send_command(msg);
}
