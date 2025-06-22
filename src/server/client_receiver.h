#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include <memory>
#include <string>

#include "common/communication_ended.h"
#include "common/game_commands_dto.h"
#include "common/message.h"
#include "common/thread.h"
#include "server/cs2d_game.h"

#define MSG_CLOSE_RECEIVER "Closing receiver thread ..."

#include "server_protocol.h"

/*
    Receives commands from the client socket and pass to the game's queue.
*/
class ClientReceiver: public Thread {
private:
    ServerProtocol& protocol;
    std::string& username;
    std::shared_ptr<CS2DGame> game;
    /*
        Push a command to the game's queue
    */
    void push_command(const GameCommandDTO& command_data);

public:
    ClientReceiver(ServerProtocol& protocol, std::string& username, std::shared_ptr<CS2DGame> game);
    /*
        Receives a command from the socket and push to the game's queue with the private method
        push_command
    */
    void receive_command();
    /*
        Run this thread calling to receive_command (blocking method)
    */
    void run() override;
    ~ClientReceiver() override;
};

#endif
