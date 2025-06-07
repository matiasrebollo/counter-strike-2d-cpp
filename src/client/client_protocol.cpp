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
        CommonProtocol(hostname, port), isAlive(true) {}

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

void ClientProtocol::send_lobby_request(const LobbyRequestDTO& request) {
    std::visit(
            [this](const auto& request_dto) {
                using T = std::decay_t<decltype(request_dto)>;
                if constexpr (std::is_same_v<T, CreateUsernameDTO>) {
                    this->send_create_username_request(request_dto);
                } else if constexpr (std::is_same_v<T, CreateGameDTO>) {
                    this->send_create_game_request(request_dto);
                } else if constexpr (std::is_same_v<T, JoinGameDTO>) {
                    this->send_join_game_request(request_dto);
                } else {
                    static_assert(always_false_v<T>, "Unhandled CommandDTO type");
                }
            },
            request);
}

void ClientProtocol::send_command(const CommandDTO& command) {
    std::visit(
            [this](const auto& d) {
                using T = std::decay_t<decltype(d)>;
                if constexpr (std::is_same_v<T, MoveUpDTO>) {
                    handle_move_up();
                } else if constexpr (std::is_same_v<T, MoveDownDTO>) {
                    handle_move_down();
                } else if constexpr (std::is_same_v<T, MoveLeftDTO>) {
                    handle_move_left();
                } else if constexpr (std::is_same_v<T, MoveRightDTO>) {
                    handle_move_right();
                } else if constexpr (std::is_same_v<T, RotateDTO>) {
                    handle_rotate(d);
                } else if constexpr (std::is_same_v<T, PlayerActionDTO>) {
                    handle_player_action();
                } else if constexpr (std::is_same_v<T, EquipPrimaryDTO>) {
                    handle_equip_primary();
                } else if constexpr (std::is_same_v<T, EquipSecondaryDTO>) {
                    handle_equip_secondary();
                } else if constexpr (std::is_same_v<T, EquipKnifeDTO>) {
                    handle_equip_knife();
                } else if constexpr (std::is_same_v<T, EquipBombDTO>) {
                    handle_equip_bomb();
                } else if constexpr (std::is_same_v<T, BuyGunDTO>) {
                    handle_buy_gun(d);
                } else if constexpr (std::is_same_v<T, BuyAmmoDTO>) {
                    handle_buy_ammo(d);
                } else {
                    static_assert(always_false_v<T>, "Unhandled CommandDTO type");
                }
            },
            command);
}

void ClientProtocol::send_create_username_request(const CreateUsernameDTO& dto) {
    this->send_byte(commandsToCode.find(CommandType::CREATE_USERNAME)->second);
    this->send_string(dto.username);
}

void ClientProtocol::send_create_game_request(const CreateGameDTO&) {
    this->send_byte(commandsToCode.find(CommandType::CREATE_GAME)->second);
}

void ClientProtocol::send_join_game_request(const JoinGameDTO& dto) {
    this->send_byte(commandsToCode.find(CommandType::JOIN_GAME)->second);
    this->send_string(dto.gamename);
}

/*
void ClientProtocol::send_select_map_request(const InternalMessage& request) {
    this->send_byte(request.map_id);
}

*/

void ClientProtocol::handle_move_up() {
    this->send_byte(CODE_MOVE);
    this->send_byte(Movement::UP + 1);
}

void ClientProtocol::handle_move_down() {
    this->send_byte(CODE_MOVE);
    this->send_byte(Movement::DOWN + 1);
}

void ClientProtocol::handle_move_left() {
    this->send_byte(CODE_MOVE);
    this->send_byte(Movement::LEFT + 1);
}

void ClientProtocol::handle_move_right() {
    this->send_byte(CODE_MOVE);
    this->send_byte(Movement::RIGHT + 1);
}

void ClientProtocol::handle_rotate(const RotateDTO& dto) {
    this->send_byte(CODE_ROTATE);
    this->send_angle(dto.angle);
}

void ClientProtocol::handle_player_action() { this->send_byte(CODE_ACTION); }

void ClientProtocol::handle_equip_primary() {
    this->send_byte(CODE_CHANGE_WEAPON);
    this->send_byte(CODE_CHOOSE_PRIMARY);
}

void ClientProtocol::handle_equip_secondary() {
    this->send_byte(CODE_CHANGE_WEAPON);
    this->send_byte(CODE_CHOOSE_SECONDARY);
}

void ClientProtocol::handle_equip_knife() {
    this->send_byte(CODE_CHANGE_WEAPON);
    this->send_byte(CODE_CHOOSE_KNIFE);
}

void ClientProtocol::handle_equip_bomb() {
    this->send_byte(CODE_CHANGE_WEAPON);
    this->send_byte(CODE_CHOOSE_BOMB);
}

void ClientProtocol::handle_buy_gun(const BuyGunDTO& dto) {
    this->send_byte(CODE_BUY_WEAPON);
    this->send_byte(this->weaponParser.getWeaponToByte(dto.gun));
}

void ClientProtocol::handle_buy_ammo(const BuyAmmoDTO& dto) {
    this->send_byte(CODE_BUY_BULLETS);
    if (dto.for_primary) {
        this->send_byte(CODE_CHOOSE_PRIMARY);
    } else {
        this->send_byte(CODE_CHOOSE_SECONDARY);
    }
    this->send_big_endian_number(dto.ammo);
}


void ClientProtocol::send_change_weapon_request(const InternalMessage& request) {
    this->send_byte(request.code_weapon_type);
}

GameDTO ClientProtocol::receive_game_dto() {
    uint8_t code = this->receive_byte();
    if (code == CODE_SEND_MAP) {
        return this->receive_map();
    } else if (code == CODE_SNAPSHOT) {
        return this->receive_snapshot();
    } else {
        return GameEnded{};
    }
}

Snapshot ClientProtocol::receive_snapshot() {
    // snasphot.bomb_status = BombStatus(this->receive_byte());
    int phase = this->receive_byte();
    size_t current_round_number = this->receive_byte();
    size_t total_rounds = this->receive_byte();
    int time_left = this->receive_byte();
    int size_ct = this->receive_byte();
    std::vector<PlayerDTO> cts = this->receive_players(size_ct);
    int size_tt = this->receive_byte();
    std::vector<PlayerDTO> tts = this->receive_players(size_tt);
    Snapshot snapshot =
            Snapshot{Phase(phase), current_round_number, total_rounds, time_left, cts, tts};
    return snapshot;
}

std::vector<PlayerDTO> ClientProtocol::receive_players(const int& size_players) {
    std::vector<PlayerDTO> players = {};
    for (int i = 0; i < size_players; i++) {
        std::string username = this->receive_string();
        int position_x = this->receive_big_endian_number();
        int position_y = this->receive_big_endian_number();
        double angle = this->receive_angle();
        int life = this->receive_byte();
        uint16_t life16 = static_cast<uint16_t>(life);
        LoadoutDTO loadout = this->receive_loadout();
        players.push_back(
                PlayerDTO{username, Vector2D(position_x, position_y), angle, life16, loadout});
    }
    return players;
}

LoadoutDTO ClientProtocol::receive_loadout() {
    uint16_t money = this->receive_big_endian_number();
    uint8_t primary_code = this->receive_byte();
    GunType primary_gun = this->weaponParser.getWeaponFromByte(primary_code);
    uint16_t primary_ammo = this->receive_big_endian_number();
    uint8_t secondary_code = this->receive_byte();
    GunType secondary_gun = this->weaponParser.getWeaponFromByte(secondary_code);
    uint16_t secondary_ammo = this->receive_big_endian_number();
    uint8_t equipped_code = this->receive_byte();
    WeaponType equipped = this->weaponParser.getWeaponTypeFromByte(equipped_code);
    return LoadoutDTO{money, primary_gun, primary_ammo, secondary_gun, secondary_ammo, equipped};
}

GameMap ClientProtocol::receive_map() {
    uint16_t size = this->receive_big_endian_number();
    return GameMap{0, 0, this->receive_map_objects(size), {}, {}, {}};
}

std::vector<MapObject> ClientProtocol::receive_map_objects(const uint8_t& size) {
    std::vector<MapObject> objects = {};
    for (int i = 0; i < size; i++) {
        uint8_t type = this->receive_byte();
        uint8_t vec_size = this->receive_byte();
        std::vector<Vector2D> positions;
        for (int j = 0; j < vec_size; j++) {
            uint16_t x = this->receive_big_endian_number();
            uint16_t y = this->receive_big_endian_number();
            positions.push_back(Vector2D(x, y));
        }
        objects.push_back({positions, type, true});
    }
    return objects;
}

void ClientProtocol::close() {
    if (!this->socket.is_stream_recv_closed()) {
        this->socket.shutdown(SHUT_RD);
    }
    if (!this->socket.is_stream_send_closed()) {
        this->socket.shutdown(SHUT_WR);
    }
    this->socket.close();
}

ClientProtocol::ClientProtocol(ClientProtocol&& other): CommonProtocol(std::move(other.socket)) {
    this->isAlive = other.isAlive;
    other.isAlive = false;
}
