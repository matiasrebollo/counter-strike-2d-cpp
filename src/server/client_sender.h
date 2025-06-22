#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include <atomic>
#include <memory>
#include <string>

#include "../common/communication_ended.h"
#include "../common/game_dto.h"
#include "../common/queue.h"

#include "server_protocol.h"

/*
    Is the owner of the client queue that lives in the server. The queue saves a variant message
   which could be one of these:

    - Snapshot: DTO which saves the current state of the game.
    - GameInitialInfo: DTO which saves the game's map info and the shop's info (for buying guns and
   bullets).
    - GameEnded: final DTO which informs the client the game is over.
*/
class ClientSender {
private:
    Queue<GameDTO> queue;
    ServerProtocol& protocol;
    std::atomic_bool keep_running;
    /*
        Pops commands from the queue and pass the message to the protocol who later
        will send it via socket. Its a blocking command.
    */
    void send_response();
    /*
        Close the queue and finish the execution of this class, returning to the ClientHandler
        method.
    */
    void game_ended();


public:
    explicit ClientSender(ServerProtocol& protocol);
    /*
        Method called by the game's thread who try push commands to the client's queue.
    */
    void send_game_dto(const GameDTO& message);
    /*
        Main method which calls to send_response, until the game is ended or it catch an error.
    */
    bool run();
    bool is_alive();
    ~ClientSender();
};

#endif
