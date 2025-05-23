#include "client_receiver.h"

#include "command.h"

ClientReceiver::ClientReceiver(ServerProtocol& protocol, const std::string& username,
                               std::shared_ptr<CS2DGame> game):
        protocol(protocol), username(username), game(game) {}

void ClientReceiver::run() {
    while (this->should_keep_running()) {
        receive_command();
        // Im sleeping inside the queue so im not burning CPU (i think)
    }
}

void ClientReceiver::receive_command() {
    CommandDTO command = this->protocol.receive_move_request();
    push_command(command);
}

void ClientReceiver::push_command(const CommandDTO& command_data) {
    this->game->push(Command::new_command(username, command_data));
}
