#include "client_receiver.h"

#include "command.h"

ClientReceiver::ClientReceiver(ServerProtocol& protocol, std::string& username,
                               std::shared_ptr<CS2DGame> game):
        protocol(protocol), username(username), game(game) {}

void ClientReceiver::run() {
    while (this->should_keep_running()) {
        try {
            this->receive_command();
            // Im sleeping inside the queue so im not burning CPU (i think)
        } catch (const CommunicationEnded& e) {
            std::cout << MSG_CLOSE_RECEIVER << std::endl;
            this->stop();
            return;
        }
    }
}

void ClientReceiver::receive_command() {
    CommandDTO command = this->protocol.receive_move_request();
    push_command(command);
}

void ClientReceiver::push_command(const CommandDTO& command_data) {
    this->game->push(Command::new_command(username, command_data));
}

ClientReceiver::~ClientReceiver() { this->join(); }
