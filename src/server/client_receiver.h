#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "../common/message.h"
#include "../common/queue.h"
#include "../common/thread.h"

#include "server_protocol.h"

class ClientReceiver: public Thread {
private:
    Queue<MessageFromClient> queue;
    ServerProtocol& protocol;
    // Me guardo la referencia de la queue del gameloop a la que voy a pushear los comandos.
    // creo que deberian ser directamente los MessageFromClient;
public:
    explicit ClientReceiver(ServerProtocol& protocol);
    void receive_command_from_client();
    void add_command_to_queue(const MessageFromClient& msg);
    void run() override;
};

#endif
