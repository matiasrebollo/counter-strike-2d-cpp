#include "server_monitor.h"

#include "cs2d_game.h"

ServerMonitor::ServerMonitor() { this->game_id = 0; }

bool ServerMonitor::create_username(const std::string& username) {
    std::unique_lock<std::mutex> lck(this->mutex);
    auto result = this->players.insert(username);
    return result.second;
}

std::shared_ptr<CS2DGame> ServerMonitor::create_new_game() {
    std::unique_lock<std::mutex> lck(this->mutex);
    std::string game_name = std::to_string(this->game_id);
    auto [it, inserted] = this->games.try_emplace(game_name, std::make_shared<CS2DGame>(game_name));
    this->game_id++;
    it->second->start();
    return it->second;
}

std::shared_ptr<CS2DGame> ServerMonitor::join_game(const std::string& gameName) {
    std::unique_lock<std::mutex> lck(this->mutex);
    auto it = this->games.find(gameName);
    return it->second;
}

CS2DGame& ServerMonitor::get_game(const std::string& gamename) {
    return *(this->games.at(gamename));
}

void ServerMonitor::manage_end_game(const std::string& gameName) { this->games.erase(gameName); }

void ServerMonitor::delete_username(const std::string& username) { this->players.erase(username); }
