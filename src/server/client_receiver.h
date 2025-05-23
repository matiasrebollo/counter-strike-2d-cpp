#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include <memory>
#include <string>

#include "common/commands_dto.h"
#include "common/message.h"
#include "common/thread.h"
#include "server/cs2d_game.h"

#include "server_protocol.h"

class ClientReceiver: public Thread {
private:
    ServerProtocol& protocol;
    const std::string username;
    std::shared_ptr<CS2DGame> game;

public:
    ClientReceiver(ServerProtocol& protocol, const std::string& username,
                   std::shared_ptr<CS2DGame> game);
    void receive_command();
    void push_command(const CommandDTO& command_data);
    void run() override;
};

#endif
