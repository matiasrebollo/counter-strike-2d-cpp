#include "client_sender.h"

ClientSender::ClientSender(ServerProtocol& protocol):
        queue(), protocol(protocol), keep_running(true) {}

void ClientSender::send_game_dto(const GameDTO& message) { this->queue.try_push(message); }

void ClientSender::send_response() {
    GameDTO msg = this->queue.pop();
    this->protocol.send_game_dto(msg);
}

void ClientSender::run() {
    while (this->keep_running) {
        this->send_response();
    }
}

void ClientSender::notify_game_ended() {
    this->queue.close();
    this->keep_running = false;
}

bool ClientSender::is_alive() { return this->keep_running; }

ClientSender::~ClientSender() {}
