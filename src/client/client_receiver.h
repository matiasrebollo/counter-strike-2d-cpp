#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "../common/communication_ended.h"
#include "../common/game_dto.h"
#include "../common/queue.h"
#include "../common/thread.h"

#include "client_protocol.h"

#define MSG_CLOSE_RECEIVER "Closing receiver thread ..."

/*
    Owner of the server -> client queue, who receives messages which updates the game's status,
    so the client can be up to date.
*/
class ClientReceiver: public Thread {
private:
    Queue<GameDTO> queue;
    ClientProtocol& protocol;

public:
    explicit ClientReceiver(ClientProtocol& protocol);
    void receive_snapshot_from_server();
    bool try_pop_game_dto(GameDTO& response);
    void close_queue();
    void run() override;
};

#endif
