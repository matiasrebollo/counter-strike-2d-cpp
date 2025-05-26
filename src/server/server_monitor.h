#ifndef SERVER_MONITOR_H
#define SERVER_MONITOR_H

#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../common/game_snapshot.h"
#include "../common/queue.h"

#include "cs2d_game.h"

struct CreateResponse {
    bool success;
    std::string gamename;
    std::shared_ptr<Queue<Snapshot>> queue;
};

class ServerMonitor {
private:
    std::mutex mutex;
    std::set<std::string> players;
    std::unordered_map<std::string, std::string> players_in_game;
    std::unordered_map<std::string, std::shared_ptr<CS2DGame>> games;
    int game_id;

public:
    ServerMonitor();
    bool create_username(const std::string& username);
    std::shared_ptr<CS2DGame> create_new_game();
    std::shared_ptr<CS2DGame> join_game(const std::string& gameName);
    void delete_username(const std::string& username);
    CS2DGame& get_game(const std::string& gameName);
    void manage_end_game(const std::string& gameName);
};

#endif
