#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>
template <class>
inline constexpr bool always_false_v = false;

#include <utility>

#include "../common/codes_parser.h"
#include "../common/commands.h"
#include "../common/commands_dto.h"
#include "../common/common_protocol.h"
#include "../common/game_dto.h"
#include "../common/lobby_request.h"
#include "../common/map_object.h"
#include "../common/message.h"
#include "../common/player_dto.h"
#include "../common/socket.h"

class ClientProtocol: public CommonProtocol, public CodesParser {
private:
    bool isAlive;

    void send_create_username_request(const CreateUsernameDTO& dto);
    void send_create_game_request(const CreateGameDTO& dto);
    void send_join_game_request(const JoinGameDTO& dto);
    void send_select_map_request(const InternalMessage& request);
    void send_buy_weapon_request(const InternalMessage& request);
    void send_buy_weapon_ammo_request(const InternalMessage& request);
    void send_aim_request(const InternalMessage& request);

    void handle_move_up();
    void handle_move_down();
    void handle_move_left();
    void handle_move_right();
    void handle_rotate(const RotateDTO& dto);
    void handle_player_action();
    void handle_equip_primary();
    void handle_equip_secondary();
    void handle_equip_knife();
    void handle_equip_bomb();
    void handle_buy_gun(const BuyGunDTO& dto);
    void handle_buy_ammo(const BuyAmmoDTO& dto);

    void send_change_weapon_request(const InternalMessage& request);

    std::vector<PlayerDTO> receive_players(const int& size_players);
    std::vector<MapObject> receive_map_objects(const uint8_t& size);

    Snapshot receive_snapshot();
    GameMap receive_map();

public:
    ClientProtocol(const std::string& hostname, const std::string& port);
    ClientProtocol(ClientProtocol&&);
    void send_command(const CommandDTO& command);
    GameDTO receive_game_dto();
    ServerResponseLobby receive_command();
    void send_lobby_request(const LobbyRequestDTO& request);
    void close();
    virtual ~ClientProtocol() {}
};

#endif
