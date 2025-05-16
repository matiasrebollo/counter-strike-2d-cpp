#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include "../common/queue.h"    
#include "../common/thread.h"
#include "client_protocol.h"
#include "../common/message.h"

class ClientSender : public Thread {
    private: 
        Queue<MessageFromClient> queue;
        ClientProtocol &protocol;
    public:
        ClientSender(ClientProtocol& protocol);
        void add_command_to_queue(const MessageFromClient& msg);
        void send_command_to_server();
        void run() override;
        ~ClientSender();
};

#endif
