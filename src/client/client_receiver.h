#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "../common/game_map.h"
#include "../common/game_snapshot.h"
#include "../common/queue.h"
#include "../common/thread.h"

#include "client_protocol.h"

class ClientReceiver: public Thread {
private:
    Queue<Snapshot> queue;
    ClientProtocol& protocol;

public:
    explicit ClientReceiver(ClientProtocol& protocol);
    GameMap receive_initial_map();
    Snapshot receive_initial_snapshot();
    void receive_snapshot_from_server();
    bool try_pop_snapshot_from_queue(Snapshot& snapshot);
    void run() override;
};

#endif
