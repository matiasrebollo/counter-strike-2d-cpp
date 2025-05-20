#include "client_protocol.h"

#include <cstdint>
#include <cstring>
#include <string>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "../common/commands.h"
#include "../common/communication_ended.h"
#include "../common/message.h"
#include "../common/vector_2d.h"

ClientProtocol::ClientProtocol(const std::string& hostname, const std::string& port):
        CommonProtocol(hostname, port), parser(), isAlive(true) {
    sendersMap[CommandType::CREATE_USERNAME] = [this](const InternalMessage& request) {
        this->send_create_username_request(request);
    };
    sendersMap[CommandType::CREATE_GAME] = [this](const InternalMessage& request) {
        return this->send_create_game_request(request);
    };
    sendersMap[CommandType::JOIN_GAME] = [this](const InternalMessage& request) {
        return this->send_join_game_request(request);
    };
    sendersMap[CommandType::SELECT_MAP] = [this](const InternalMessage& request) {
        return this->send_select_map_request(request);
    };
    sendersMap[CommandType::BUY_WEAPON] = [this](const InternalMessage& request) {
        return this->send_buy_weapon_request(request);
    };
    sendersMap[CommandType::BUY_AMMO] = [this](const InternalMessage& request) {
        return this->send_buy_weapon_ammo_request(request);
    };
    sendersMap[CommandType::AIM] = [this](const InternalMessage& request) {
        return this->send_aim_request(request);
    };
    sendersMap[CommandType::MOVE] = [this](const InternalMessage& request) {
        return this->send_move_request(request);
    };
    sendersMap[CommandType::CHANGE_WEAPON] = [this](const InternalMessage& request) {
        return this->send_change_weapon_request(request);
    };
}

ServerResponseLobby ClientProtocol::receive_command() {
    // aca para la etapa de lobby recibo:
    // rta de pedido de crear nombre de usuario
    // rta de pedido de crear partida
    // rta de pedio de joinear partida
    // notificacion de empezó partida -> aca lanzó los hilos y queues
    uint8_t code = this->receive_byte();
    ServerResponseLobby response =
            ServerResponseLobby{this->codeToCommands.find(code)->second, false, ""};
    if (this->codeToCommands.find(code)->second != CommandType::GAME_STARTED) {
        response.success = this->receive_byte();
        if (response.commandType == CommandType::CREATE_GAME) {
            response.game_name = this->receive_string();
            // recibo el nombre de la partida que el server me generó automáticamente
        }
    }
    return response;
}

void ClientProtocol::send_command(const MessageFromClient& request) {
    InternalMessage msg = this->parser.ParseMessage(request);
    this->send_byte(msg.code_message);
    if (request.commandType == CommandType::SHOOT ||
        request.commandType == CommandType::PLANT_BOMB ||
        request.commandType == CommandType::DEFUSE_BOMB) {
        return;
    }
    this->sendersMap.find(request.commandType)->second(msg);
}

void ClientProtocol::send_create_username_request(const InternalMessage& request) {
    this->send_string(request.s);
}

void ClientProtocol::send_create_game_request(const InternalMessage& request) {
    this->send_byte(request.size_players);
    this->send_select_skins_request(request);
}

void ClientProtocol::send_join_game_request(const InternalMessage& request) {
    this->send_string(request.s);
    this->send_select_skins_request(request);
}

void ClientProtocol::send_select_skins_request(const InternalMessage& request) {
    this->send_byte(request.skin_id_tt);
    this->send_byte(request.skin_id_ct);
}

void ClientProtocol::send_select_map_request(const InternalMessage& request) {
    this->send_byte(request.map_id);
}

void ClientProtocol::send_buy_weapon_request(const InternalMessage& request) {
    this->send_byte(request.code_weapon);
}

void ClientProtocol::send_buy_weapon_ammo_request(const InternalMessage& request) {
    this->send_byte(request.code_weapon_type);
    this->send_big_endian_number(request.bullets);
}

void ClientProtocol::send_aim_request(const InternalMessage& request) {
    this->send_byte(request.pos_x);
    this->send_byte(request.pos_y);
}

void ClientProtocol::send_move_request(const InternalMessage& request) {
    this->send_byte(request.direction);
}

void ClientProtocol::send_change_weapon_request(const InternalMessage& request) {
    this->send_byte(request.code_weapon_type);
}

Snapshot ClientProtocol::receive_snapshot() {
    // snasphot.phase = Phase(this->receive_byte());
    // snasphot.round_number = this->receive_byte();
    // snasphot.bomb_status = BombStatus(this->receive_byte());
    // snasphot.timer = this->receive_byte();
    int size_players = this->receive_byte();
    Snapshot snapshot = Snapshot{this->receive_players(size_players)};
    // int size_bullets = this->receive_byte();
    // snapshot.bullets = this->receive_bullets(size_bullets);
    return snapshot;
}

std::vector<PlayerDTO> ClientProtocol::receive_players(const int& size_players) {
    std::vector<PlayerDTO> players = {};
    for (int i = 0; i < size_players; i++) {
        int position_x = this->receive_byte();
        int position_y = this->receive_byte();
        int direction_x = this->receive_byte();
        int direction_y = this->receive_byte();
        int life = this->receive_byte();
        uint16_t life16 = static_cast<uint16_t>(life);
        players.push_back(PlayerDTO{Vector2D(position_x, position_y),
                                    Vector2D(direction_x, direction_y), life16});
    }
    return players;
}

/*
std::vector<Bullet> ClientProtocol::receive_bullets(const int& size_bullets) {
    std::vector<Bullet> bullets = {};
    for (int i = 0; i < size_bullets; i++) {
        uint8_t id = this->ReceiveByte();
        uint8_t pos_x = this->ReceiveByte();
        uint8_t pos_y = this->ReceiveByte();
        uint8_t dir_x = this->ReceiveByte();
        uint8_t dir_y = this->ReceiveByte();
        bullets.push_back(Bullet(id, pos_x, pos_y, dir_x, dir_y));
    }
    return bullets;
}
*/

GameMap ClientProtocol::receive_map() {
    this->receive_byte();
    uint8_t size = this->receive_byte();
    return GameMap{this->receive_map_objects(size)};
}

std::vector<MapObject> ClientProtocol::receive_map_objects(const uint8_t& size) {
    std::vector<MapObject> objects = {};
    for (int i = 0; i < size; i++) {
        uint8_t type = this->receive_byte();
        uint8_t x = this->receive_byte();
        uint8_t y = this->receive_byte();
        uint8_t height = this->receive_byte();
        uint8_t width = this->receive_byte();
        objects.push_back(MapObject{Vector2D(x, y), height, width, MapObjectType(type)});
    }
    return objects;
}

void ClientProtocol::Close() {
    this->socket.shutdown(SHUT_RDWR);
    this->socket.close();
}

ClientProtocol::~ClientProtocol() {}
