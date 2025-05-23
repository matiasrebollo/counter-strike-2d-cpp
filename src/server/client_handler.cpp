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
        my_game("") {}

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
        LobbyRequestDTO dto = this->protocol.receive_lobby_request();

        // aca en msg en caso de crear o joinear tengo las skins, en algun lado deberia guardarlo,
        // asumo que pasarlo al server_monitor -> game_monitor -> el game lo guarda
        this->manage_lobby_request(dto);
    }
}

MessageFromClient ClientHandler::ReceivePlay() { return this->protocol.receive_command(); }

void ClientHandler::sendLobbyResponse(const CommandType& command, const bool& success,
                                      const std::string& game_name) {
    this->protocol.send_lobby_message(ServerResponseLobby{command, success, game_name});
}

void ClientHandler::manage_lobby_request(const LobbyRequestDTO& dto) {
    std::visit(
            [this](auto&& request) {
                using T = std::decay_t<decltype(request)>;
                if constexpr (std::is_same_v<T, CreateUsernameDTO>) {
                    this->manage_create_username(request);
                } else if constexpr (std::is_same_v<T, CreateGameDTO>) {
                    this->manage_create_game(request);
                } else if constexpr (std::is_same_v<T, JoinGameDTO>) {
                    this->manage_join_game(request);
                }
            },
            dto);
}

void ClientHandler::manage_create_username(const CreateUsernameDTO& dto) {
    bool success = this->server_monitor.CreateUsername(dto.username);
    if (success) {
        this->username = dto.username;
    }
    this->sendLobbyResponse(CommandType::CREATE_USERNAME, success, "");
}

void ClientHandler::manage_create_game(const CreateGameDTO&) {
    std::shared_ptr<CS2DGame> game = this->server_monitor.CreateNewGame();
    if (!this->isInGame() && this->username != "") {
        this->my_game = game->id;
        this->is_in_game = true;
        this->sendLobbyResponse(CommandType::CREATE_GAME, true, this->my_game);
        ClientReceiver(this->protocol, this->username, game).start();
        ClientSender sender(this->protocol);
        game->new_player(username, sender);
        sender.run();
        return;
    }
    this->sendLobbyResponse(CommandType::CREATE_GAME, false, "");
}

void ClientHandler::manage_join_game(const JoinGameDTO& dto) {
    std::shared_ptr<CS2DGame> game = this->server_monitor.JoinGame(dto.gamename);
    if (!this->isInGame() && this->username != "") {
        this->is_in_game = true;
        this->my_game = dto.gamename;
        this->sendLobbyResponse(CommandType::JOIN_GAME, true, "");
        ClientReceiver(this->protocol, this->username, game).start();
        ClientSender sender(this->protocol);
        game->new_player(username, sender);
        sender.run();
        return;
    }
    this->sendLobbyResponse(CommandType::JOIN_GAME, false, "");
}

std::string ClientHandler::GetUsername() { return this->username; }

void ClientHandler::manageEndGame() { this->server_monitor.ManageEndGame(this->my_game); }

void ClientHandler::kill() { this->stop(); }

bool ClientHandler::isInGame() { return this->is_in_game && this->my_game != ""; }
