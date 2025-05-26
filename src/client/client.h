#ifndef CLIENT_H
#define CLIENT_H

#include <optional>
#include <string>

#include "lobby/lobby.h"

class Client {
public:
    Client();

    void run(int argc, char* argv[]);
};

#endif  // CLIENT_H
