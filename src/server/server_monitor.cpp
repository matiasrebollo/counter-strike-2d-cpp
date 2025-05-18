#include "server_monitor.h"

#include "cs2d_game.h"

ServerMonitor::ServerMonitor() { this->game_id = 0; }

bool ServerMonitor::CreateUsername(const std::string& username) {
    std::unique_lock<std::mutex> lck(this->mutex);
    auto result = this->players.insert(username);
    return result.second;
}

CreateResponse ServerMonitor::CreateNewGame(const std::string& username) {
    std::unique_lock<std::mutex> lck(this->mutex);
    std::string game_name = std::to_string(this->game_id);
    auto [it, inserted] = this->games.try_emplace(game_name, CS2DGame());
    CS2DGame& game = *(it->second);
    std::shared_ptr<Queue<Snapshot>> queue = game.new_player(username);
    this->game_id++;
    return CreateResponse{true, game_name, queue};
}

CreateResponse ServerMonitor::JoinGame(const std::string& gameName, const std::string& username) {
    std::unique_lock<std::mutex> lck(this->mutex);
    auto it = this->games.find(gameName);

    CS2DGame& game = *(it->second);
    std::shared_ptr<Queue<Snapshot>> queue = game.new_player(username);
    return CreateResponse{true, gameName, queue};
}

void ServerMonitor::MakePlayGame(const std::string& gameName) {
    auto it = this->games.find(gameName);
    if (it != this->games.end()) {
        // it->second.MakePlayGame(client);
    }
}

CS2DGame& ServerMonitor::GetGame(const std::string& gamename) {
    return *(this->games.at(gamename));
}

void ServerMonitor::ManageEndGame(const std::string& gameName) { this->games.erase(gameName); }
