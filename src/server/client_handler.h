#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>

#include "../common/socket.h"
#include "../common/thread.h"
#include "common/message.h"

#include "server_monitor.h"
#include "server_protocol.h"

#define MSG_CLOSE_SENDER "Closing sender thread ..."

class ClientHandler: public Thread {
private:
    ServerProtocol protocol;
    ServerMonitor& server_monitor;
    std::string username;
    bool is_in_game;
    std::string my_game;
    bool game_ended;
    bool is_dead;

    void send_lobby_response(const CommandType& command, const ResponseStatus& status,
                             const std::string& game_name);
    void manage_lobby_request(const LobbyRequestDTO& dto);
    void manage_create_username(const CreateUsernameDTO& dto);
    void manage_create_game(const CreateGameDTO&);
    void manage_join_game(const JoinGameDTO& dto);
    bool in_game();
    void launch_lobby();

public:
    ClientHandler(Socket&& socket, ServerMonitor& server_monitor);
    std::string get_username();
    void run() override;
    void kill();
};

#endif
