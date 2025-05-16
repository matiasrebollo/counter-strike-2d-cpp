#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../common/codes_parser.h"
#include "../common/commands.h"
#include "../common/common_protocol.h"
#include "../common/game_snapshot.h"
#include "../common/message.h"
#include "../common/socket.h"

#define CODE_SUCCESS 0x01
#define CODE_FAIL 0x00

class ServerProtocol: public CommonProtocol, public CodesParser {
private:
    std::unordered_map<bool, uint8_t> codeSuccessResponse;
    std::unordered_map<CommandType, std::function<MessageFromClient(const CommandType& command)>>
            commandsManagers;

    MessageFromClient receive_create_username_request(const CommandType& command);
    MessageFromClient receive_create_game_request(const CommandType& command);
    MessageFromClient receive_join_game_request(const CommandType& command);
    MessageFromClient receive_select_map_request(const CommandType& command);
    MessageFromClient receive_buy_weapon_request(const CommandType& command);
    MessageFromClient receive_buy_weapon_ammo_request(const CommandType& command);
    MessageFromClient receive_aim_request(const CommandType& command);
    MessageFromClient receive_move_request(const CommandType& command);
    MessageFromClient receive_shoot_request(const CommandType& command);
    MessageFromClient receive_change_weapon_request(const CommandType& command);
    MessageFromClient receive_plant_bomb_request(const CommandType& command);
    MessageFromClient receive_defuse_bomb_request(const CommandType& command);

    MessageFromClient receive_select_skins_request(const CommandType& command);

    MessageFromClient initialize_message(const CommandType& command);

    void send_players(const std::vector<PlayerDTO>& players);
    // void send_bullets(const std::vector<Bullet>& bullets);
public:
    explicit ServerProtocol(Socket&& socket);
    void send_lobby_message(const ServerResponseLobby& msg);
    void send_start_game(const ServerResponseLobby& msg);
    void send_snapshot(const Snapshot& snapshot);
    MessageFromClient receive_command(void);
    void kill();
    ~ServerProtocol();
};

#endif
