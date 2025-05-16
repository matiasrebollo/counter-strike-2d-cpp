#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <string>
#include <unordered_map>

#include "../common/socket.h"
#include "../common/thread.h"
#include "server_protocol.h"

#include "server_monitor.h"
#include "server_protocol.h"

class ClientHandler: public Thread {
private:
    ServerProtocol protocol;
    std::atomic_bool keepTalking;
    std::atomic_bool isAlive;
    std::unordered_map<CommandType, std::function<void(const MessageFromClient& request)>>
            managersMap;
    ServerMonitor& server_monitor;
    std::string username;
    bool is_in_game;
    std::string my_game;

    void sendLobbyResponse(const CommandType& command, const bool& success,
                           const std::string& game_name);
    void manageCommand(const MessageFromClient& msg);
    void manageCreateUsername(const MessageFromClient& msg);
    void manageCreateGame(const MessageFromClient& msg);
    void manageJoinGame(const MessageFromClient& msg);
    void manageEndGame();
    bool isInGame();
    MessageFromClient ReceiveMessage();
    void launchLobby();
    void launchGame();

public:
    ClientHandler(Socket&& socket, ServerMonitor& server_monitor);
    void SendStatusGame(const MessageFromServer& msg);
    std::string GetUsername();
    MessageFromClient ReceivePlay();
    void run() override;
    void kill();
};

#endif
