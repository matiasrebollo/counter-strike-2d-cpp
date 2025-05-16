#include "game_monitor.h"

#include <string>

#include "client_handler.h"

GameMonitor::GameMonitor(const std::string& creatorUsername): gameReady(false) {
    this->cts.emplace(creatorUsername);
}

bool GameMonitor::AddPlayer(const std::string& playerUsername) {
    if (this->cts.size() < PLAYERS_CT && this->cts.size() < this->tts.size()) {
        this->cts.emplace(playerUsername);
        this->updateGameIsReady();
        return true;
    } else if (this->tts.size() < PLAYERS_TT && this->tts.size() < this->cts.size()) {
        this->tts.emplace(playerUsername);
        this->updateGameIsReady();
        return true;
    }
    return false;
}

void GameMonitor::updateGameIsReady() {
    if (this->cts.size() == PLAYERS_CT && this->tts.size() == PLAYERS_TT) {
        this->gameReady = true;
        this->gameIsReady.notify_all();
    }
}

void GameMonitor::WaitPlayers() {
    std::unique_lock<std::mutex> lck(this->mutex);
    this->gameIsReady.wait(lck, [&]() { return this->gameReady; });
    // aca deberia avisarle a todos que empezó. como hago?
}

bool GameMonitor::isFinished() { return false; }
