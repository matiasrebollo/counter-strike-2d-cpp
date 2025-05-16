#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include "../common/message.h"
#include "../common/queue.h"
#include "../common/thread.h"

#include "client_protocol.h"

class ClientSender: public Thread {
private:
    Queue<MessageFromClient> queue;
    ClientProtocol& protocol;

public:
    explicit ClientSender(ClientProtocol& protocol);
    void add_command_to_queue(const MessageFromClient& msg);
    void send_command_to_server();
    void run() override;
    ~ClientSender();
};

#endif
