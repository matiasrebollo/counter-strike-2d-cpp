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

/*
    Owner of the server -> client socket. Implements a binary protocol to send the data to the
   client.
*/
class ServerProtocol: public CommonProtocol {
private:
    std::unordered_map<CommandType, std::function<LobbyRequestDTO()>> lobbyCommandManagers;

    /*
        Receivers of lobby's requests
    */
    CreateUsernameDTO receive_create_username_request();
    CreateGameDTO receive_create_game_request();
    JoinGameDTO receive_join_game_request();
    /*
        Receivers of game's requests
    */
    RotateDTO receive_rotate();
    GameCommandDTO receive_movement_request();
    GameCommandDTO receive_change_weapon_request();
    GameCommandDTO receive_buy_weapon_request();
    GameCommandDTO receive_buy_ammo_request();

    /*
        Main and helper methods for sending an snapshot
    */
    void send_snapshot(const Snapshot& snapshot);
    void send_players(const std::vector<PlayerDTO>& players);
    void send_shot(const PlayerDTO& player);
    void send_loadout(const LoadoutDTO& loadout);
    void send_bomb_position(const std::optional<Vector2D<int>>& bomb_position);
    void send_current_round_winner(const std::optional<Team>& current_round_winner);

    void send_game_init_info(const GameInitialInfoDTO& dto);
    void send_end_game(const GameEnded& message);
    void send_items(const std::vector<ItemDTO>& items);
    void send_particular_item(const DroppedGunDTO& dropped_gun);
    void send_particular_item(const DroppedBombDTO& bomb);

public:
    explicit ServerProtocol(std::unique_ptr<Socket> socket);
    ServerProtocol(ServerProtocol&& other) noexcept;
    ServerProtocol& operator=(ServerProtocol&& other) noexcept;
    /*
        Sends responses to the lobby requests of a client-
    */
    void send_lobby_message(const ServerResponseLobby& msg);
    /*
        Sends a game dto via socket. It can be a:
        - GameInitialInfoDTO
        - Snapshot
        - GameEnded
    */
    void send_game_dto(const GameDTO& message);
    /*
        Receives game's requests from the client, like I want to move, or shoot, for example.
    */
    GameCommandDTO receive_client_game_request();
    LobbyRequestDTO receive_lobby_request();
    void kill();

    ~ServerProtocol();
};

#endif
