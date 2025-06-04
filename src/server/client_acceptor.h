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

class ClientAcceptor: public Thread {
private:
    Socket acceptor;
    std::vector<ClientHandler*> clients;
    ServerMonitor server_monitor;

public:
    explicit ClientAcceptor(const std::string& port);
    void run() override;
    void reap();
    void clear();
};

#endif
