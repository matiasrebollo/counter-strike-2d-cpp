#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

#include "../common/commands.h"
#include "../common/commands_dto.h"
#include "../common/common_protocol.h"
#include "../common/game_dto.h"
#include "../common/lobby_request.h"
#include "../common/message.h"
#include "../common/socket.h"

#define CODE_SUCCESS 0x01
#define CODE_FAIL 0x00

class ServerProtocol: public CommonProtocol {
private:
    std::unordered_map<bool, uint8_t> codeSuccessResponse;
    std::unordered_map<CommandType, std::function<LobbyRequestDTO()>> lobbyCommandManagers;

    CreateUsernameDTO receive_create_username_request();
    CreateGameDTO receive_create_game_request();
    JoinGameDTO receive_join_game_request();

    RotateDTO receive_rotate();
    CommandDTO receive_movement_request();
    CommandDTO receive_change_weapon_request();
    CommandDTO receive_buy_weapon_request();
    CommandDTO receive_buy_ammo_request();

    void send_snapshot(const Snapshot& snapshot);
    void send_map(const GameMap& map);
    void send_players(const std::vector<PlayerDTO>& players);
    void send_loadout(const LoadoutDTO& loadout);
    void send_end_game(const GameEnded& message);

public:
    explicit ServerProtocol(Socket&& socket);
    void send_lobby_message(const ServerResponseLobby& msg);
    void send_start_game(const ServerResponseLobby& msg);
    void send_game_dto(const GameDTO& message);
    CommandDTO receive_client_request();
    LobbyRequestDTO receive_lobby_request();
    void kill();
    ServerProtocol(ServerProtocol&& other);
    ~ServerProtocol();
};

#endif
