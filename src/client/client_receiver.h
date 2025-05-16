#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

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
    void receive_snapshot_from_server();
    Snapshot pop_snapshot_from_queue();
    void run() override;
};

#endif
