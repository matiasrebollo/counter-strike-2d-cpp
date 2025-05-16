#include "client.h"

#include <arpa/inet.h>

#include "../common/communication_ended.h"

Client::Client(const std::string& hostname, const std::string& port):
        protocol(hostname, port), isAlive(true) {}

int Client::Run() {
    std::string line;

    while (std::getline(std::cin, line)) {
        try {
            this->SendMessage(line);
            ServerResponseMessage message = this->ReceiveMessage();
            this->updateIsAlive(message.serverResponse);
            this->PrintMessageReceived(message.serverResponse);
            if (!this->isAlive) {
                break;
            }
        } catch (const CommunicationEnded& e) {
            break;
        }
    }
    this->protocol.Close();
    return 0;
}

int Client::SendMessage(const std::string& message) {
    MessageFromClient request = this->parser.ParseRequest(message);
    this->protocol.send_command(request);
    return 0;
}

void Client::PrintMessageReceived(const std::string& message) { std::cout << message; }

ServerResponseMessage Client::ReceiveMessage() { return this->protocol.receive_command(); }

void Client::updateIsAlive(const std::string& response) {}

Client::~Client() {}
