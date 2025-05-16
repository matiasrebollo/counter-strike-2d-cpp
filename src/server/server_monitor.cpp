#include "server_monitor.h"

ServerMonitor::ServerMonitor() { this->game_id = 0; }

bool ServerMonitor::CreateUsername(const std::string& username) {
    std::unique_lock<std::mutex> lck(this->mutex);
    auto result = this->players.insert(username);
    return result.second;
}

std::tuple<bool, std::string> ServerMonitor::CreateNewGame(const std::string& username) {
    std::unique_lock<std::mutex> lck(this->mutex);
    std::string game_name = std::to_string(this->game_id);
    auto result = this->gameMonitors.try_emplace(game_name, username);
    // despues acá utilizar UUID, tengo que buscar alguna library que lo haga
    this->game_id++;
    return std::make_tuple(result.second, game_name);
}

bool ServerMonitor::JoinGame(const std::string& gameName, const std::string& username) {
    std::unique_lock<std::mutex> lck(this->mutex);
    auto it = this->gameMonitors.find(gameName);
    if (it != this->gameMonitors.end()) {
        return it->second.AddPlayer(username);
    } else {
        return false;
    }
}

void ServerMonitor::MakePlayGame(const std::string& gameName) {
    auto it = this->gameMonitors.find(gameName);
    if (it != this->gameMonitors.end()) {
        //it->second.MakePlayGame(client);
    }
}

GameMonitor& ServerMonitor::GetGameMonitor(const std::string& gameName) {
    return this->gameMonitors.at(gameName);
}

void ServerMonitor::ManageEndGame(const std::string& gameName) {
    this->gameMonitors.erase(gameName);
}
