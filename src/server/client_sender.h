#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include <atomic>
#include <memory>
#include <string>

#include "../common/communication_ended.h"
#include "../common/game_dto.h"
#include "../common/queue.h"

#include "server_protocol.h"

class ClientSender {
private:
    Queue<GameDTO> queue;
    ServerProtocol& protocol;
    std::atomic_bool keep_running;
    void send_response();
    void game_ended();


public:
    explicit ClientSender(ServerProtocol& protocol);
    void send_game_dto(const GameDTO& message);
    bool run();
    bool is_alive();
    ~ClientSender();
};

#endif
