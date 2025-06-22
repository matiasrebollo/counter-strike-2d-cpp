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

/*
    Thread in charge to manage all the client's lobby request
*/
class ClientHandler: public Thread {
private:
    ServerProtocol protocol;
    ServerMonitor& server_monitor;
    std::string username;
    bool is_in_game;
    std::string my_game;
    bool game_ended;
    bool is_dead;

    /*
        Send the status of the requested lobby command.
    */
    void send_lobby_response(const CommandType& command, const ResponseStatus& status,
                             const std::string& game_name);
    /*
        Manage the client's lobby request
    */
    void manage_lobby_request(const LobbyRequestDTO& dto);
    /*
        Manage the creation of a username calling to the server monitor method which use
        a mutex.
    */
    void manage_create_username(const CreateUsernameDTO& dto);
    /*
        Manage the creation of a game calling to the server monitor method which
        use a mutex. If the client don't have ausername or its already in a game it
        sends an appropiate response.
    */
    void manage_create_game(const CreateGameDTO&);
    /*
        Manage the join to a game calling to the server monitor's method which
        use a mutex.
        It fails in these cases, each one send an aproppiate message explaining the error:
        - User don't have a name yet.
        - Game doesn't exist.
        - The game is already dead because all the clients had left it.
        - The game is full of players.abort
        - A user with the username is already in the game. It only happens if someone used to
        have the username as the actual client, and had left the game and also the server =>
        in the moment the new client joined the server, nobody have his name, but in the game
        world is still somebody with this username.
    */
    void manage_join_game(const JoinGameDTO& dto);

    bool in_game();
    /*
        Manages all the lobby requests
    */
    void launch_lobby();

public:
    ClientHandler(Socket&& socket, ServerMonitor& server_monitor);
    std::string get_username();
    void run() override;
    void kill();
};

#endif
