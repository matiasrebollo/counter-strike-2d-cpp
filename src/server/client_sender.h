#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include "../common/queue.h"
#include "../common/snapshot.h"
#include "server_protocol.h"
#include <atomic>

class ClientSender {
    private: 
        Queue<Snapshot> queue;
        ServerProtocol& protocol;   
        std::atomic_bool keep_running;
    public:
        ClientSender(ServerProtocol& protocol);
        void add_snapshot_to_queue(const Snapshot& snapshot);
        void send_snapshot_to_client();
        void run();
        ~ClientSender();
};

#endif
