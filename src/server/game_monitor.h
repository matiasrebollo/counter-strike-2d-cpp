#ifndef GAME_MONITOR_H
#define GAME_MONITOR_H

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include "server_protocol.h"

#define PLAYERS_TT 2
#define PLAYERS_CT 2  // aca realmente deberia leer del yaml para determinar esto

class ClientHandler;

class GameMonitor {
private:
    std::mutex mutex;
    std::set<std::string> tts;
    std::set<std::string> cts;
    std::condition_variable gameIsReady;
    bool gameReady;

    void updateGameIsReady();

public:
    explicit GameMonitor(const std::string& creatorUsername);
    bool AddPlayer(const std::string& playerUsername);
    void WaitPlayers();

    bool isFinished();
};

#endif
