#ifndef CLIENT_ACCEPTOR_H
#define CLIENT_ACCEPTOR_H

#include <string>
#include <vector>

#include "../common/communication_ended.h"
#include "../common/liberror.h"
#include "../common/socket.h"
#include "../common/thread.h"

#include "client_handler.h"
#include "server_monitor.h"

#define MSG_CLIENT_JOINED(id) "Client " + id + " has been joined!"

/*
    Accepts new client with the acceptor socket and in case it accepts, pass the new socket to the
    client handler with the purpose client-server communicates via it.
    It always uses the same socket for accept new ones clients.
    Pass the reference to the server monitor (who manages the shared resources) to the new clients.
*/
class ClientAcceptor: public Thread {
private:
    Socket acceptor;
    std::vector<ClientHandler*> clients;
    ServerMonitor server_monitor;
    void clear();

public:
    explicit ClientAcceptor(const std::string& port);
    void run() override;
    void reap();
    void close_acceptor();
};

#endif
