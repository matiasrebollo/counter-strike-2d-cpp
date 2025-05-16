#ifndef CLIENT_H
#define CLIENT_H

#include <cstdint>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../common/message.h"

#include "client_parser.h"
#include "client_protocol.h"

class Client {
private:
    ClientProtocol protocol;
    ClientParser parser;
    bool isAlive;

    int SendMessage(const std::string& message);
    ServerResponseMessage ReceiveMessage();
    void updateIsAlive(const std::string& response);
    bool isFinalMessage(const std::string& response, const std::string& finalMessage);
    void PrintMessageReceived(const std::string& message);

public:
    Client(const std::string& hostname, const std::string& port);
    int Run();
    ~Client();
};

#endif
