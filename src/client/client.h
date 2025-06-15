#ifndef CLIENT_H
#define CLIENT_H

#include <optional>
#include <string>
#include <unordered_map>

#include "../common/skins.h"
#include "lobby/lobby.h"

class Client {
private:
public:
    Client();

    void run(int argc, char* argv[]);
};

#endif  // CLIENT_H
