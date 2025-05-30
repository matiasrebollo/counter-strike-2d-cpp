#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include <atomic>
#include <memory>
#include <string>

#include "../common/communication_ended.h"
#include "../common/queue.h"
#include "../common/server_message.h"

#include "server_protocol.h"

#define MSG_CLOSE_SENDER "Player " + this->username + " has disconnected! Closing sender thread ..."

class ClientSender {
private:
    Queue<GameResponseDTO> queue;
    ServerProtocol& protocol;
    std::atomic_bool keep_running;
    void send_response();

public:
    explicit ClientSender(ServerProtocol& protocol);
    void send_game_dto(const GameResponseDTO& message);
    void run();
    ~ClientSender();
};

#endif
