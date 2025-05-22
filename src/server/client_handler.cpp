#include "client_handler.h"

#include <utility>
#include <vector>

#include <sys/socket.h>

#include "client_receiver.h"
#include "client_sender.h"
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
    this->is_in_game = false;
    while (this->should_keep_running()) {
        this->launchLobby();
    }
    this->manageEndGame();
    this->stop();
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

void ClientHandler::sendLobbyResponse(const CommandType& command, const bool& success,
                                      const std::string& game_name) {
    this->protocol.send_lobby_message(ServerResponseLobby{command, success, game_name});
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
    std::shared_ptr<CS2DGame> game = this->server_monitor.CreateNewGame();
    if (!this->isInGame() && this->username != "") {
        this->my_game = game->id;
        this->is_in_game = true;
        this->sendLobbyResponse(msg.commandType, true, this->my_game);
        ClientReceiver(this->protocol, game).run();  // este es el que es el thread
        ClientSender sender(this->protocol);
        game->new_player(username, sender);
        // enviar mensaje empezó partida
        // aca deberia lanzar el otro hilo y las queues
        return;
    }
    this->sendLobbyResponse(msg.commandType, false, "");
}

void ClientHandler::manageJoinGame(const MessageFromClient& msg) {
    std::shared_ptr<CS2DGame> game = this->server_monitor.JoinGame(msg.s);
    if (!this->isInGame() && this->username != "") {
        this->is_in_game = true;
        this->my_game = msg.s;
        this->sendLobbyResponse(msg.commandType, true, "");
        ClientReceiver(this->protocol, game).run();
        ClientSender sender(this->protocol);
        game->new_player(username, sender);
        // enviar mensaje empezó partida
        // aca deberia lanzar el otro hilo y las queues
        return;
    }
    this->sendLobbyResponse(msg.commandType, false, "");
}

std::string ClientHandler::GetUsername() { return this->username; }

void ClientHandler::manageEndGame() { this->server_monitor.ManageEndGame(this->my_game); }

void ClientHandler::kill() { this->stop(); }

bool ClientHandler::isInGame() { return this->is_in_game && this->my_game != ""; }
