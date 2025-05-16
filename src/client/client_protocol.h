#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../common/codes_parser.h"
#include "../common/commands.h"
#include "../common/common_protocol.h"
#include "../common/message.h"
#include "../common/socket.h"

#include "client_protocol_parser.h"

class ClientProtocol: public CommonProtocol, public CodesParser {
private:
    ClientProtocolParser parser;
    bool isAlive;
    std::unordered_map<CommandType, std::function<void(const InternalMessage& request)>> sendersMap;

    void send_create_username_request(const InternalMessage& request);
    void send_create_game_request(const InternalMessage& request);
    void send_join_game_request(const InternalMessage& request);
    void send_select_skins_request(const InternalMessage& request);
    void send_select_map_request(const InternalMessage& request);
    void send_buy_weapon_request(const InternalMessage& request);
    void send_buy_weapon_ammo_request(const InternalMessage& request);
    void send_aim_request(const InternalMessage& request);
    void send_move_request(const InternalMessage& request);
    void send_change_weapon_request(const InternalMessage& request);

    /*
    std::vector<Bullet> receive_bullets(const int& size_bullets);
    std::vector<Player> receive_players(const int& size_players);
    */

public:
    ClientProtocol(const std::string& hostname, const std::string& port);
    void send_command(const MessageFromClient& request);
    ServerResponseLobby receive_command();
    // Snapshot receive_snapshot();
    void Close();
    ~ClientProtocol();
};

#endif
