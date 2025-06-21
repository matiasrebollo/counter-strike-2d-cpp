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


std::shared_ptr<CS2DGame> ServerMonitor::create_new_game(const std::string& username,
                                                         const std::string& map_filename,
                                                         std::shared_ptr<ClientSender> sender) {
    std::unique_lock<std::mutex> lck(this->mutex);
    std::string game_name = std::to_string(this->game_id);
    auto [it, inserted] =
            this->games.try_emplace(game_name, std::make_shared<CS2DGame>(game_name, map_filename));
    this->game_id++;
    it->second->add_player(username, sender);
    return it->second;
}


CS2DGame& ServerMonitor::get_game(const std::string& gamename) {
    return *(this->games.at(gamename));
}

void ServerMonitor::manage_end_game(const std::string& gameName) { this->games.erase(gameName); }

void ServerMonitor::delete_username(const std::string& username) { this->players.erase(username); }

std::shared_ptr<CS2DGame> ServerMonitor::join_game(const std::string& gameName,
                                                   const std::string& username,
                                                   std::shared_ptr<ClientSender> sender) {
    std::unique_lock<std::mutex> lck(this->mutex);
    auto it = this->games.find(gameName);
    if (it == this->games.end()) {
        return nullptr;
    } else {
        if (!it->second->is_alive()) {
            throw GameDeadException();
        }
        it->second->add_player(username, sender);
        return it->second;
    }
}

void ServerMonitor::reap_games() {
    for (auto it = games.begin(); it != games.end();) {
        std::shared_ptr<CS2DGame> game = it->second;
        if (!game->is_alive()) {
            std::string deleted_game = game->id;
            game->join();
            it = games.erase(it);
            std::cout << MSG_GAME_JOINED(deleted_game) << std::endl;
        } else {
            ++it;
        }
    }
}

void ServerMonitor::kill_games() {
    for (auto it = games.begin(); it != games.end();) {
        std::shared_ptr<CS2DGame> game = it->second;
        std::string deleted_game = game->id;
        game->stop();
        game->join();
        it = games.erase(it);
        std::cout << MSG_GAME_JOINED(deleted_game) << std::endl;
    }
}
