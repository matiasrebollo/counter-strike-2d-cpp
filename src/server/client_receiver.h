#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include <memory>
#include <string>

#include "common/commands_dto.h"
#include "common/communication_ended.h"
#include "common/message.h"
#include "common/thread.h"
#include "server/cs2d_game.h"

#define MSG_CLOSE_RECEIVER "Closing receiver thread ..."

#include "server_protocol.h"

class ClientReceiver: public Thread {
private:
    ServerProtocol& protocol;
    std::string& username;
    std::shared_ptr<CS2DGame> game;

public:
    ClientReceiver(ServerProtocol& protocol, std::string& username, std::shared_ptr<CS2DGame> game);
    void receive_command();
    void push_command(const CommandDTO& command_data);
    void run() override;
    ~ClientReceiver() override;
};

#endif
