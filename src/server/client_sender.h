#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include <atomic>
#include <memory>
#include <string>

#include "../common/communication_ended.h"
#include "../common/game_map.h"
#include "../common/game_snapshot.h"
#include "../common/queue.h"

#include "server_protocol.h"

#define MSG_CLOSE_SENDER "Player " + this->username + " has disconnected! Closing sender thread ..."

class ClientSender {
private:
    Queue<Snapshot> queue;
    ServerProtocol& protocol;
    std::atomic_bool keep_running;
    void send_snapshot();

public:
    explicit ClientSender(ServerProtocol& protocol);
    void push(const Snapshot& snapshot);
    void send_map(const GameMap& map);
    void run();
    ~ClientSender();
};

#endif
