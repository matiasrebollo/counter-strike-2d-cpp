#ifndef SERVER_MONITOR_H
#define SERVER_MONITOR_H

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <set>
#include <tuple>

#include "game_monitor.h"

class ServerMonitor {
private:
    std::mutex mutex;
    std::set<std::string> players;
    std::map<std::string, std::string> players_in_game;
    std::map<std::string, GameMonitor> gameMonitors;
    int game_id;

public:
    ServerMonitor();
    bool CreateUsername(const std::string& username);
    std::tuple<bool, std::string> CreateNewGame(const std::string& username);
    bool JoinGame(const std::string& gameName, const std::string& username);
    void MakePlayGame(const std::string& gameName, ClientHandler& client);
    GameMonitor& GetGameMonitor(const std::string& gameName);
    void ManageEndGame(const std::string& gameName);
};

#endif
