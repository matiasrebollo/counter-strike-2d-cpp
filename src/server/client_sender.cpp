#include "client_sender.h"

ClientSender::ClientSender(ServerProtocol& protocol):
        queue(), protocol(protocol), keep_running(true) {}

void ClientSender::send_game_dto(const GameDTO& message) {
    try {
        this->queue.try_push(message);
    } catch (const std::exception& e) {
        std::cout << "Intente pushear a queue cerrada " << e.what() << std::endl;
    }
}

void ClientSender::send_response() {
    GameDTO msg = this->queue.pop();
    this->protocol.send_game_dto(msg);
}

void ClientSender::run() {
    while (this->keep_running) {
        try {
            this->send_response();
        } catch (const ClosedQueue& e) {
            std::cout << "Intente popear de queue cerrada" << std::endl;
            break;
        }
    }
    this->queue.close();
}

ClientSender::~ClientSender() {}
