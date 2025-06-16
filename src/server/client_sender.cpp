#include "client_sender.h"

ClientSender::ClientSender(ServerProtocol& protocol):
        queue(), protocol(protocol), keep_running(true) {}

void ClientSender::send_game_dto(const GameDTO& message) { this->queue.try_push(message); }

void ClientSender::send_response() {
    GameDTO msg = this->queue.pop();
    if (std::holds_alternative<GameEnded>(msg)) {
        this->game_ended();
    }
    try {
        this->protocol.send_game_dto(msg);
    } catch (const CommunicationEnded& e) {
        this->game_ended();
        throw;
    }
}

bool ClientSender::run() {
    while (this->keep_running) {
        this->send_response();
    }
    return true;
}

void ClientSender::game_ended() {
    this->queue.close();
    this->keep_running = false;
}

bool ClientSender::is_alive() { return this->keep_running; }

ClientSender::~ClientSender() {}
