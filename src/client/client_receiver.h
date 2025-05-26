#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "../common/communication_ended.h"
#include "../common/game_map.h"
#include "../common/game_snapshot.h"
#include "../common/queue.h"
#include "../common/thread.h"

#include "client_protocol.h"

#define MSG_CLOSE_RECEIVER "Closing receiver thread ..."

class ClientReceiver: public Thread {
private:
    Queue<Snapshot> queue;
    ClientProtocol& protocol;

public:
    explicit ClientReceiver(ClientProtocol& protocol);
    GameMap receive_initial_map();
    Snapshot receive_initial_snapshot();
    void receive_snapshot_from_server();
    Snapshot pop_snapshot_from_queue();
    void close_queue();
    void run() override;
};

#endif
