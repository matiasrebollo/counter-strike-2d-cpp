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
    /*
        Starts the thread acceptor with a socket in the port specified in the
        command line arg. If the port is occupied the program will end.
    */
    int Run();
};

#endif
