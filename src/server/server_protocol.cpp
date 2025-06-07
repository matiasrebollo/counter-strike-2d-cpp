#include "server_protocol.h"

#include <cstring>
#include <numbers>
#include <utility>

#include <sys/socket.h>
#include <sys/types.h>

#include "../common/player_dto.h"
#include "../common/skins.h"

ServerProtocol::ServerProtocol(Socket&& socket):
        CommonProtocol(std::move(socket)),
        codeSuccessResponse({{true, CODE_SUCCESS}, {false, CODE_FAIL}}) {
    lobbyCommandManagers[CommandType::CREATE_USERNAME] = [this]() -> LobbyRequestDTO {
        return receive_create_username_request();
    };
    lobbyCommandManagers[CommandType::CREATE_GAME] = [this]() -> LobbyRequestDTO {
        return receive_create_game_request();
    };
    lobbyCommandManagers[CommandType::JOIN_GAME] = [this]() -> LobbyRequestDTO {
        return receive_join_game_request();
    };
}

void ServerProtocol::send_lobby_message(const ServerResponseLobby& msg) {
    this->send_byte(this->commandsToCode.find(msg.commandType)->second);
    this->send_byte(this->codeSuccessResponse.find(msg.success)->second);
    if (msg.game_name != "") {
        this->send_string(msg.game_name);
    }
}

void ServerProtocol::send_start_game(const ServerResponseLobby& msg) {
    this->send_byte(this->commandsToCode.find(msg.commandType)->second);
}

void ServerProtocol::send_game_dto(const GameDTO& response) {
    std::visit(
            [this](const auto& response) {
                using T = std::decay_t<decltype(response)>;
                if constexpr (std::is_same_v<T, GameMap>) {
                    this->send_map(response);
                } else if constexpr (std::is_same_v<T, Snapshot>) {
                    this->send_snapshot(response);
                } else if constexpr (std::is_same_v<T, GameEnded>) {
                    this->send_end_game(response);
                } else {
                    static_assert(always_false_v<T>, "Unhandled ServerResponseDTO type");
                }
            },
            response);
}

void ServerProtocol::send_map(const GameMap& map) {
    this->send_byte(CODE_SEND_MAP);
    this->send_big_endian_number(map.map_objects.size());
    for (auto object: map.map_objects) {
        this->send_byte(object.type);
        this->send_byte(object.positions.size());
        for (auto vec: object.positions) {
            this->send_big_endian_number(vec.x);
            this->send_big_endian_number(vec.y);
        }
    }
}


void ServerProtocol::send_snapshot(const Snapshot& snapshot) {
    this->send_byte(CODE_SNAPSHOT);
    this->send_byte(snapshot.phase);
    this->send_byte(snapshot.current_round_number);
    this->send_byte(snapshot.total_rounds);
    // this->send_byte(snapshot.bomb_status);
    this->send_byte(snapshot.time_left);
    this->send_byte(snapshot.ct.size());
    this->send_players(snapshot.ct);
    this->send_byte(snapshot.tt.size());
    this->send_players(snapshot.tt);
}

void ServerProtocol::send_players(const std::vector<PlayerDTO>& players) {
    for (const auto& player: players) {
        this->send_string(player.username);
        this->send_big_endian_number(player.position.x);
        this->send_big_endian_number(player.position.y);
        this->send_angle(player.orientation);
        this->send_byte(player.life);
        this->send_loadout(player.loadout);
    }
}

void ServerProtocol::send_loadout(const LoadoutDTO& loadout) {
    this->send_big_endian_number(loadout.money);
    this->send_byte(this->weaponParser.getWeaponToByte(loadout.primary_gun));
    this->send_big_endian_number(loadout.primary_ammo);
    this->send_byte(this->weaponParser.getWeaponToByte(loadout.secondary_gun));
    this->send_big_endian_number(loadout.secondary_ammo);
    this->send_byte(this->weaponParser.getWeaponTypeToByte(loadout.equipped));
}

void ServerProtocol::send_end_game(const GameEnded&) { this->send_byte(CODE_ENDGAME); }

LobbyRequestDTO ServerProtocol::receive_lobby_request() {
    uint8_t commandCode = this->receive_byte();
    CommandType command = this->codeToCommands.find(commandCode)->second;
    return this->lobbyCommandManagers.find(command)->second();
}

CommandDTO ServerProtocol::receive_client_request() {
    uint8_t code = this->receive_byte();
    switch (code) {
        case CODE_ROTATE:
            return this->receive_rotate();
        case CODE_MOVE:
            return this->receive_movement_request();
        case CODE_ACTION:
            return PlayerActionDTO{};
        case CODE_CHANGE_WEAPON:
            return this->receive_change_weapon_request();
        case CODE_BUY_WEAPON:
            return this->receive_buy_weapon_request();
        case CODE_BUY_BULLETS:
            return this->receive_buy_ammo_request();
        default:
            throw std::runtime_error("Command not recognised");
    }
}

CommandDTO ServerProtocol::receive_movement_request() {
    uint8_t code_movement = this->receive_byte();
    switch (static_cast<Movement>(code_movement - 1)) {
        case Movement::UP:
            return MoveUpDTO{};
        case Movement::DOWN:
            return MoveDownDTO{};
        case Movement::LEFT:
            return MoveLeftDTO{};
        case Movement::RIGHT:
            return MoveRightDTO{};
        default:
            throw std::runtime_error("Unknown move code");
    }
}

CommandDTO ServerProtocol::receive_change_weapon_request() {
    uint8_t code_to_equip = this->receive_byte();
    switch (code_to_equip) {
        case CODE_CHOOSE_KNIFE:
            return EquipKnifeDTO{};
        case CODE_CHOOSE_PRIMARY:
            return EquipPrimaryDTO{};
        case CODE_CHOOSE_SECONDARY:
            return EquipSecondaryDTO{};
        case CODE_CHOOSE_BOMB:
            return EquipBombDTO{};
        default:
            throw std::runtime_error("Unkown equipment code");
    }
}

CommandDTO ServerProtocol::receive_buy_weapon_request() {
    uint8_t gun_code = this->receive_byte();
    GunType gun = this->weaponParser.getWeaponFromByte(gun_code);
    return BuyGunDTO{gun};
}

CommandDTO ServerProtocol::receive_buy_ammo_request() {
    uint8_t primary_code = this->receive_byte();
    uint16_t ammo = this->receive_big_endian_number();
    bool primary = false;
    if (primary_code == CODE_CHOOSE_PRIMARY) {
        primary = true;
    }
    return BuyAmmoDTO{ammo, primary};
}


CreateUsernameDTO ServerProtocol::receive_create_username_request() {
    CreateUsernameDTO dto;
    std::string username = this->receive_string();
    dto.username = username;
    return dto;
}

CreateGameDTO ServerProtocol::receive_create_game_request() { return CreateGameDTO{}; }

JoinGameDTO ServerProtocol::receive_join_game_request() {
    return JoinGameDTO{this->receive_string()};
}


RotateDTO ServerProtocol::receive_rotate() { return RotateDTO{this->receive_angle()}; }

void ServerProtocol::kill() {
    if (!this->socket.is_stream_recv_closed()) {
        this->socket.shutdown(SHUT_RD);
    }
    if (!this->socket.is_stream_send_closed()) {
        this->socket.shutdown(SHUT_WR);
    }
    this->socket.close();
}

ServerProtocol::~ServerProtocol() {}
