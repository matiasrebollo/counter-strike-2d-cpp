#include "server_monitor.h"

#include "cs2d_game.h"

ServerMonitor::ServerMonitor() { this->game_id = 0; }

bool ServerMonitor::create_username(const std::string& username) {
    std::unique_lock<std::mutex> lck(this->mutex);
    if (username == "") {
        return false;
    } else {
        auto result = this->players.insert(username);
        return result.second;
    }
}


std::shared_ptr<CS2DGame> ServerMonitor::CreateNewGame(const std::string& username) {
    std::unique_lock<std::mutex> lck(this->mutex);
    std::string game_name = std::to_string(this->game_id);
    auto [it, inserted] = this->games.try_emplace(game_name, std::make_shared<CS2DGame>(game_name));
    this->game_id++;
    it->second->add_player(username);
    return it->second;
}


CS2DGame& ServerMonitor::get_game(const std::string& gamename) {
    return *(this->games.at(gamename));
}

void ServerMonitor::manage_end_game(const std::string& gameName) { this->games.erase(gameName); }

void ServerMonitor::delete_username(const std::string& username) { this->players.erase(username); }

std::shared_ptr<CS2DGame> ServerMonitor::JoinGame(const std::string& gameName,
                                                  const std::string& username) {
    std::unique_lock<std::mutex> lck(this->mutex);
    auto it = this->games.find(gameName);
    if (it->second->can_add_player()) {
        it->second->add_player(username);
        return it->second;
    } else {
        return nullptr;  // devuelve nullptr si no se pudo unir al juego al jugador
    }
}
