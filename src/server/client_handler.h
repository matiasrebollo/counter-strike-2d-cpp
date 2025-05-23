#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>

#include "../common/socket.h"
#include "../common/thread.h"

#include "server_monitor.h"
#include "server_protocol.h"

class ClientHandler: public Thread {
private:
    ServerProtocol protocol;
    ServerMonitor& server_monitor;
    std::string username;
    bool is_in_game;
    std::string my_game;

    void sendLobbyResponse(const CommandType& command, const bool& success,
                           const std::string& game_name);
    void manage_lobby_request(const LobbyRequestDTO& dto);
    void manage_create_username(const CreateUsernameDTO& dto);
    void manage_create_game(const CreateGameDTO&);
    void manage_join_game(const JoinGameDTO& dto);
    void manageEndGame();
    bool isInGame();
    MessageFromClient ReceiveMessage();
    void launchLobby();

public:
    ClientHandler(Socket&& socket, ServerMonitor& server_monitor);
    std::string GetUsername();
    MessageFromClient ReceivePlay();
    void run() override;
    void kill();
};

#endif
