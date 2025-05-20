#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include <atomic>
#include <memory>

#include "../common/game_map.h"
#include "../common/game_snapshot.h"
#include "../common/queue.h"

#include "server_protocol.h"

class ClientSender {
private:
    Queue<Snapshot> queue;
    ServerProtocol& protocol;
    std::atomic_bool keep_running;

    void run();
    void send_snapshot_to_client();

public:
    explicit ClientSender(ServerProtocol& protocol);
    void add_snapshot_to_queue(const Snapshot& snapshot);
    void send_map(const GameMap& map);
};

#endif
