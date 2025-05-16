#include "client_handler.h"

#include <utility>
#include <vector>

#include <sys/socket.h>

#include "server_monitor.h"

ClientHandler::ClientHandler(Socket&& socket, ServerMonitor& server_monitor):
        protocol(std::move(socket)),
        server_monitor(server_monitor),
        username(""),
        is_in_game(false),
        my_game("") {
    managersMap[CommandType::CREATE_USERNAME] = [this](const MessageFromClient& request) {
        return manageCreateUsername(request);
    };
    managersMap[CommandType::CREATE_GAME] = [this](const MessageFromClient& request) {
        return manageCreateGame(request);
    };
    managersMap[CommandType::JOIN_GAME] = [this](const MessageFromClient& request) {
        return manageJoinGame(request);
    };
}

void ClientHandler::run() {
    this->_is_alive = this->_keep_running = true;
    this->is_in_game = false;
    while (this->_keep_running) {
        this->launchLobby();
        this->launchGame();
    }
    this->manageEndGame();
    this->_is_alive = false;
    this->protocol.kill();
}

void ClientHandler::launchLobby() {
    while (!this->isInGame()) {
        MessageFromClient msg = this->protocol.receive_command();
        // aca en msg en caso de crear o joinear tengo las skins, en algun lado deberia guardarlo,
        // asumo que pasarlo al server_monitor -> game_monitor -> el game lo guarda
        this->manageCommand(msg);
    }
}

MessageFromClient ClientHandler::ReceivePlay() { return this->protocol.receive_command(); }

void ClientHandler::launchGame() {
    while (!this->server_monitor.GetGameMonitor(this->my_game).isFinished()) {
        this->server_monitor.MakePlayGame(this->my_game, *this);
    }
    this->_keep_running = false;
}

void ClientHandler::sendLobbyResponse(const CommandType& command, const bool& success,
                                      const std::string& game_name) {
    this->protocol.send_lobby_message(ServerResponseLobby{command, success, game_name});
}

void ClientHandler::SendStatusGame(const MessageFromServer& msg) {
    this->protocol.SendMessage(msg);
}

void ClientHandler::manageCommand(const MessageFromClient& msg) {
    this->managersMap.find(msg.commandType)->second(msg);
}

void ClientHandler::manageCreateUsername(const MessageFromClient& msg) {
    bool success = this->server_monitor.CreateUsername(msg.s);
    if (success) {
        this->username = msg.s;
    }
    this->sendLobbyResponse(msg.commandType, success, "");
}

void ClientHandler::manageCreateGame(const MessageFromClient& msg) {
    auto response = this->server_monitor.CreateNewGame(this->GetUsername());
    if (!this->isInGame() && std::get<0>(response) && this->username != "") {
        this->my_game = msg.s;
        this->is_in_game = true;
        this->sendLobbyResponse(msg.commandType, true, this->my_game);
        // DEBO MANDAR EL CODIGO DE LA PARTIDA
        this->server_monitor.GetGameMonitor(std::get<1>(response)).WaitPlayers();
        this->protocol.send_start_game(ServerResponseLobby{CommandType::GAME_STARTED});
        // enviar mensaje empezó partida
        // aca deberia lanzar el otro hilo y las queues
        return;
    }
    this->sendLobbyResponse(msg.commandType, false, "");
}

void ClientHandler::manageJoinGame(const MessageFromClient& msg) {
    bool success = this->server_monitor.JoinGame(msg.s, this->GetUsername());
    if (!this->isInGame() && success && this->username != "") {
        this->is_in_game = true;
        this->my_game = msg.s;
        this->sendLobbyResponse(msg.commandType, true, "");
        this->server_monitor.GetGameMonitor(msg.s).WaitPlayers();
        this->protocol.send_start_game(ServerResponseLobby{CommandType::GAME_STARTED});
        // enviar mensaje empezó partida
        // aca deberia lanzar el otro hilo y las queues
        return;
    }
    this->sendLobbyResponse(msg.commandType, false, "");
}

std::string ClientHandler::GetUsername() { return this->username; }

void ClientHandler::manageEndGame() { this->server_monitor.ManageEndGame(this->my_game); }

void ClientHandler::kill() { this->_keep_running = false; }

bool ClientHandler::isInGame() { return this->is_in_game && this->my_game != ""; }
