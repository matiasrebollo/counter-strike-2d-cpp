#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "../common/commands.h"
#include "../common/common_protocol.h"
#include "../common/game_commands_dto.h"
#include "../common/game_dto.h"
#include "../common/loadout_dto.h"
#include "../common/lobby_request.h"
#include "../common/map_object.h"
#include "../common/message.h"
#include "../common/player_dto.h"

#ifdef TESTS
#include "../common/mock_socket.h"
using Socket = MockSocket;
#else
#include "../common/socket.h"
using Socket = RealSocket;
#endif

class ClientProtocol: public CommonProtocol {
private:
    bool isAlive;

    virtual void send_create_username_request(const CreateUsernameDTO& dto);
    virtual void send_create_game_request(const CreateGameDTO& dto);
    virtual void send_join_game_request(const JoinGameDTO& dto);

    void handle_move(const MoveDTO& dto);
    void handle_rotate(const RotateDTO& dto);
    void handle_player_action(const PlayerActionDTO& dto);
    void handle_defuse_bomb(const DefuseBombDTO& dto);
    void handle_equip_primary();
    void handle_equip_secondary();
    void handle_equip_knife();
    void handle_equip_bomb();
    void handle_buy_gun(const BuyGunDTO& dto);
    void handle_buy_ammo(const BuyAmmoDTO& dto);

    std::vector<PlayerDTO> receive_players(const int& size_players);
    std::optional<ShotDTO> receive_shot();
    LoadoutDTO receive_loadout();
    std::optional<Vector2D<int>> receive_bomb_position();
    std::optional<Team> receive_current_round_winner();

    Snapshot receive_snapshot();
    GameInitialInfoDTO receive_game_initial_info();
    std::vector<MapObject> receive_map_objects(const uint8_t& size);
    std::set<Vector2D<int>> receive_sites(const uint16_t& size);
    std::unordered_map<GunType, int> receive_gun_prices(const uint8_t& size);
    std::unordered_map<GunType, int> receive_gun_clips_size(const uint8_t& size);

public:
    explicit ClientProtocol(std::unique_ptr<Socket> socket);

    void send_command(const GameCommandDTO& command);
    GameDTO receive_game_dto();
    ServerResponseLobby receive_server_response_lobby();
    void send_lobby_request(const LobbyRequestDTO& request);
    void close();
    virtual ~ClientProtocol() {}
};

#endif
