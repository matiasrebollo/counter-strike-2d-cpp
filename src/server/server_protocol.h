#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

#include "../common/commands.h"
#include "../common/common_protocol.h"
#include "../common/game_commands_dto.h"
#include "../common/game_dto.h"
#include "../common/lobby_request.h"
#include "../common/message.h"
#include "../common/socket.h"

class ServerProtocol: public CommonProtocol {
private:
    std::unordered_map<CommandType, std::function<LobbyRequestDTO()>> lobbyCommandManagers;

    CreateUsernameDTO receive_create_username_request();
    CreateGameDTO receive_create_game_request();
    JoinGameDTO receive_join_game_request();

    RotateDTO receive_rotate();
    GameCommandDTO receive_movement_request();
    GameCommandDTO receive_change_weapon_request();
    GameCommandDTO receive_buy_weapon_request();
    GameCommandDTO receive_buy_ammo_request();

    void send_snapshot(const Snapshot& snapshot);
    void send_game_init_info(const GameInitialInfoDTO& dto);
    void send_players(const std::vector<PlayerDTO>& players);
    void send_shot(const PlayerDTO& player);
    void send_loadout(const LoadoutDTO& loadout);
    void send_bomb_position(const std::optional<Vector2D<int>>& bomb_position);
    void send_current_round_winner(const std::optional<Team>& current_round_winner);
    void send_end_game(const GameEnded& message);

public:
    explicit ServerProtocol(std::unique_ptr<Socket> socket);
    ServerProtocol(ServerProtocol&& other) noexcept;
    ServerProtocol& operator=(ServerProtocol&& other) noexcept;

    void send_lobby_message(const ServerResponseLobby& msg);
    void send_start_game(const ServerResponseLobby& msg);
    void send_game_dto(const GameDTO& message);
    GameCommandDTO receive_client_game_request();
    LobbyRequestDTO receive_lobby_request();
    void kill();

    ~ServerProtocol();
};

#endif
