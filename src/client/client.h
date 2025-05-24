#ifndef CLIENT_H
#define CLIENT_H

#include <optional>
#include <string>

#include "lobby/lobby.h"

#include "client_protocol.h"

#define MSG_NO_PROTOCOL "You have to connect yourself to a server to play :)"
#define MSG_NO_USERNAME "You have to enter your username to login in our server"
#define MSG_NO_GAME "You have to create a game or join one to play!"

class Client {
private:
    bool validate_qt_results(Lobby& lobby);
    void print_message(const std::string& s);

public:
    Client();

    void run(int argc, char* argv[]);
};

#endif  // CLIENT_H
