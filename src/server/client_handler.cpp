#include "client_handler.h"

#include <utility>
#include <vector>

#include <sys/socket.h>

#include "client_receiver.h"
#include "client_sender.h"
#include "game_full_exception.h"
#include "player_in_game_exception.h"
#include "server_monitor.h"

ClientHandler::ClientHandler(Socket&& socket, ServerMonitor& server_monitor):
        protocol(std::make_unique<Socket>(std::move(socket))),
        server_monitor(server_monitor),
        username(""),
        is_in_game(false),
        my_game(""),
        game_ended(false),
        is_dead(false) {}

void ClientHandler::run() {
    this->is_in_game = false;
    while (this->should_keep_running() && !this->game_ended) {
        try {
            this->launch_lobby();
        } catch (const CommunicationEnded& e) {
            break;
        } catch (const ClosedQueue& e) {
            break;
        }
    }
    this->kill();
}

void ClientHandler::launch_lobby() {
    while (!this->in_game()) {
        LobbyRequestDTO dto = this->protocol.receive_lobby_request();
        this->manage_lobby_request(dto);
    }
}

void ClientHandler::send_lobby_response(const CommandType& command, const ResponseStatus& status,
                                        const std::string& game_name) {
    this->protocol.send_lobby_message(ServerResponseLobby{command, status, game_name});
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
    ResponseStatus status = ResponseStatus::USERNAME_IN_USE;
    if (success) {
        this->server_monitor.delete_username(this->get_username());
        this->username = dto.username;
        status = ResponseStatus::SUCCESS;
    }
    this->send_lobby_response(CommandType::CREATE_USERNAME, status, "");
}

void ClientHandler::manage_create_game(const CreateGameDTO& dto) {
    if (this->in_game() || this->username == "") {
        this->send_lobby_response(CommandType::CREATE_GAME, ResponseStatus::WITHOUT_USERNAME, "");
    } else {
        std::shared_ptr<ClientSender> sender = std::make_shared<ClientSender>(this->protocol);
        std::shared_ptr<CS2DGame> game =
                this->server_monitor.create_new_game(this->username, dto.map_file_name, sender);
        game->start();
        this->my_game = game->id;
        this->send_lobby_response(CommandType::CREATE_GAME, ResponseStatus::SUCCESS, this->my_game);
        this->is_in_game = true;
        ClientReceiver receiver(this->protocol, this->username, game);
        receiver.start();
        this->game_ended = sender->run();
    }
}

void ClientHandler::manage_join_game(const JoinGameDTO& dto) {
    if (this->in_game() || this->get_username() == "") {
        this->send_lobby_response(CommandType::JOIN_GAME, ResponseStatus::WITHOUT_USERNAME, "");
    } else {
        std::shared_ptr<ClientSender> sender = std::make_shared<ClientSender>(this->protocol);
        try {
            std::shared_ptr<CS2DGame> game =
                    this->server_monitor.join_game(dto.gamename, this->username, sender);
            if (game == nullptr) {
                this->send_lobby_response(CommandType::JOIN_GAME, ResponseStatus::GAME_NOT_EXIST,
                                          "");
            } else {
                this->is_in_game = true;
                this->my_game = dto.gamename;
                this->send_lobby_response(CommandType::JOIN_GAME, ResponseStatus::SUCCESS, "");
                ClientReceiver receiver(this->protocol, this->username, game);
                receiver.start();
                this->game_ended = sender->run();
            }
        } catch (const GameFullException& e) {
            this->send_lobby_response(CommandType::JOIN_GAME, ResponseStatus::GAME_IS_FULL, "");
        } catch (const PlayerAlreadyInGameException& e) {
            this->send_lobby_response(CommandType::JOIN_GAME,
                                      ResponseStatus::USERNAME_ALREADY_IN_GAME, "");
        }
    }
}

std::string ClientHandler::get_username() { return this->username; }

bool ClientHandler::in_game() { return this->is_in_game && this->my_game != ""; }

void ClientHandler::kill() {
    if (!this->is_dead) {
        std::cout << MSG_CLOSE_SENDER << std::endl;
        this->protocol.kill();
        this->stop();
        this->server_monitor.delete_username(this->get_username());
        this->is_dead = true;
    }
}
