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
        try {
            this->launch_lobby();
        } catch (const CommunicationEnded& e) {
            std::cout << MSG_CLIENT_DISCONNECTED << std::endl;
            this->stop();
        }
    }
    this->server_monitor.delete_username(this->get_username());
    this->protocol.kill();
}

void ClientHandler::launch_lobby() {
    while (!this->in_game()) {
        LobbyRequestDTO dto = this->protocol.receive_lobby_request();
        this->manage_lobby_request(dto);
    }
}

void ClientHandler::send_lobby_response(const CommandType& command, const bool& success,
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
    bool success = this->server_monitor.create_username(dto.username);
    if (success) {
        this->server_monitor.delete_username(this->get_username());
        // usa erase, si existe el nombre actual lo va a sacar del map, si no no hace nada
        this->username = dto.username;
    }
    this->send_lobby_response(CommandType::CREATE_USERNAME, success, "");
}

void ClientHandler::manage_create_game(const CreateGameDTO&) {
    std::shared_ptr<CS2DGame> game = this->server_monitor.CreateNewGame(this->username);
    if (!this->isInGame() && this->username != "") {
        this->my_game = game->id;
        this->is_in_game = true;
        this->send_lobby_response(CommandType::CREATE_GAME, true, this->my_game);
        ClientReceiver receiver(this->protocol, this->username, game);
        auto sender = std::make_shared<ClientSender>(this->protocol);
        game->add_player_sender(username, sender);
        receiver.start();
        sender->run();
        return;
    }
    this->send_lobby_response(CommandType::CREATE_GAME, false, "");
}

void ClientHandler::manage_join_game(const JoinGameDTO& dto) {
    std::shared_ptr<CS2DGame> game = this->server_monitor.JoinGame(dto.gamename, this->username);
    if (game != nullptr && !this->isInGame() && this->username != "") {
        this->is_in_game = true;
        this->my_game = dto.gamename;
        this->send_lobby_response(CommandType::JOIN_GAME, true, "");
        ClientReceiver receiver(this->protocol, this->username, game);
        auto sender = std::make_shared<ClientSender>(this->protocol);
        game->add_player_sender(username, sender);
        receiver.start();
        sender->run();
        return;
    }
    this->send_lobby_response(CommandType::JOIN_GAME, false, "");
}

std::string ClientHandler::get_username() { return this->username; }

bool ClientHandler::in_game() { return this->is_in_game && this->my_game != ""; }
