#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include "../common/communication_ended.h"
#include "../common/message.h"
#include "../common/queue.h"
#include "../common/thread.h"

#include "client_protocol.h"

#define MSG_CLOSE_SENDER "Closing sender thread ..."

/*
    Thread where the game command queue lives. It keeps the commands and while it pops commands
    from the queue it calls to the protocol for send them.
*/
class ClientSender: public Thread {
private:
    Queue<GameCommandDTO> queue;
    ClientProtocol& protocol;

public:
    explicit ClientSender(ClientProtocol& protocol);
    void add_command_to_queue(const GameCommandDTO& dto);
    void send_command_to_server();
    void close_queue();
    void run() override;
};

#endif
