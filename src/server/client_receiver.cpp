#include "client_receiver.h"

ClientReceiver::ClientReceiver(Queue<MessageFromClient>& queue, ServerProtocol& protocol) : queue(queue), protocol(protocol) {}

void ClientReceiver::run() {
    this->_keep_running = true;
    while (this->should_keep_running()) {
        receive_command_from_client();
        // Im sleeping inside the queue so im not burning CPU (i think)
    }
}

void ClientReceiver::receive_command_from_client() {
    MessageFromClient msg = this->protocol.receive_command();
    add_command_to_queue(msg);
}

void ClientReceiver::add_command_to_queue(const MessageFromClient& msg) {
    this->queue.push(msg);
}
