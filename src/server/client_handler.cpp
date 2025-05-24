#include "client_handler.h"

#include <utility>
#include <vector>

#include <sys/socket.h>

#include "../common/communication_ended.h"

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
    try {
        while (this->should_keep_running()) {
            this->launchLobby();
        }
    } catch (const CommunicationEnded& e) {
        std::cout << MSG_CLIENT_DISCONNECTED << std::endl;
    }
    this->server_monitor.delete_username(this->get_username());
    this->stop();
    this->protocol.kill();
}

void ClientHandler::launchLobby() {
    while (!this->in_game()) {
        LobbyRequestDTO dto = this->protocol.receive_lobby_request();
        this->manage_lobby_request(dto);
    }
}

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
        this->server_monitor.delete_username(this->get_username());
        // usa erase, si existe el nombre actual lo va a sacar del map, si no no hace nada
        this->username = dto.username;
    }
    this->sendLobbyResponse(CommandType::CREATE_USERNAME, success, "");
}

void ClientHandler::manage_create_game(const CreateGameDTO&) {
    std::shared_ptr<CS2DGame> game = this->server_monitor.CreateNewGame();
    if (!this->in_game() && this->username != "") {
        this->my_game = game->id;
        this->is_in_game = true;
        this->sendLobbyResponse(CommandType::CREATE_GAME, true, this->my_game);
        ClientReceiver receiver(this->protocol, this->username, game);
        ClientSender sender(this->protocol);
        game->new_player(username, sender);
        receiver.start();
        sender.run();
        return;
    }
    this->sendLobbyResponse(CommandType::CREATE_GAME, false, "");
}

void ClientHandler::manage_join_game(const JoinGameDTO& dto) {
    std::shared_ptr<CS2DGame> game = this->server_monitor.JoinGame(dto.gamename);
    if (!this->in_game() && this->username != "") {
        this->is_in_game = true;
        this->my_game = dto.gamename;
        this->sendLobbyResponse(CommandType::JOIN_GAME, true, "");
        ClientReceiver receiver(this->protocol, this->username, game);
        ClientSender sender(this->protocol);
        game->new_player(username, sender);
        receiver.start();
        sender.run();
        return;
    }
    this->sendLobbyResponse(CommandType::JOIN_GAME, false, "");
}

std::string ClientHandler::get_username() { return this->username; }

void ClientHandler::kill() { this->stop(); }

bool ClientHandler::in_game() { return this->is_in_game && this->my_game != ""; }
