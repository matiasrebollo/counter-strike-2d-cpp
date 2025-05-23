#include "server_monitor.h"

#include "cs2d_game.h"

ServerMonitor::ServerMonitor() { this->game_id = 0; }

bool ServerMonitor::CreateUsername(const std::string& username) {
    std::unique_lock<std::mutex> lck(this->mutex);
    auto result = this->players.insert(username);
    return result.second;
}

std::shared_ptr<CS2DGame> ServerMonitor::CreateNewGame() {
    std::unique_lock<std::mutex> lck(this->mutex);
    std::string game_name = std::to_string(this->game_id);
    auto [it, inserted] = this->games.try_emplace(game_name, std::make_shared<CS2DGame>(game_name));
    this->game_id++;
    it->second->start();
    return it->second;
}

std::shared_ptr<CS2DGame> ServerMonitor::JoinGame(const std::string& gameName) {
    std::unique_lock<std::mutex> lck(this->mutex);
    auto it = this->games.find(gameName);
    return it->second;
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
