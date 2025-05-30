#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "../common/communication_ended.h"
#include "../common/game_response.h"
#include "../common/queue.h"
#include "../common/thread.h"

#include "client_protocol.h"

#define MSG_CLOSE_RECEIVER "Closing receiver thread ..."

class ClientReceiver: public Thread {
private:
    Queue<GameResponseDTO> queue;
    ClientProtocol& protocol;

public:
    explicit ClientReceiver(ClientProtocol& protocol);
    void receive_snapshot_from_server();
    bool try_pop_game_response(GameResponseDTO& response);
    void close_queue();
    void run() override;
};

#endif
