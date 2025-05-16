#ifndef SERVER_H
#define SERVER_H

#include <string>

#include "client_acceptor.h"
#define MESSAGE_KILL_SERVER 'q'

class Server {
private:
    ClientAcceptor acceptor;

public:
    explicit Server(const std::string& port);
    int Run();
};

#endif
