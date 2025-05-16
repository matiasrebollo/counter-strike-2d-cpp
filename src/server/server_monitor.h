#ifndef SERVER_MONITOR_H
#define SERVER_MONITOR_H

#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "game_monitor.h"

class ServerMonitor {
private:
    std::mutex mutex;
    std::set<std::string> players;
    std::unordered_map<std::string, std::string> players_in_game;
    std::unordered_map<std::string, GameMonitor> gameMonitors;
    int game_id;

public:
    ServerMonitor();
    bool CreateUsername(const std::string& username);
    std::tuple<bool, std::string> CreateNewGame(const std::string& username);
    bool JoinGame(const std::string& gameName, const std::string& username);
    // void MakePlayGame(const std::string& gameName, ClientHandler& client);
    void MakePlayGame(const std::string& gameName);
    GameMonitor& GetGameMonitor(const std::string& gameName);
    void ManageEndGame(const std::string& gameName);
};

#endif
