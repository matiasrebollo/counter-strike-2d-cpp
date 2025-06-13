#include "server_protocol.h"

#include <cstring>
#include <memory>
#include <numbers>
#include <utility>

#include <sys/socket.h>
#include <sys/types.h>

#include "../common/player_dto.h"
#include "../common/skins.h"

ServerProtocol::ServerProtocol(std::unique_ptr<Socket> socket): CommonProtocol(std::move(socket)) {
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

ServerProtocol::ServerProtocol(ServerProtocol&& other) noexcept:
        CommonProtocol(std::move(other.socket)),
        lobbyCommandManagers(std::move(other.lobbyCommandManagers)) {}

ServerProtocol& ServerProtocol::operator=(ServerProtocol&& other) noexcept {
    if (this != &other) {
        CommonProtocol::operator=(std::move(other));
        lobbyCommandManagers = std::move(other.lobbyCommandManagers);
    }
    return *this;
}

void ServerProtocol::send_lobby_message(const ServerResponseLobby& msg) {
    this->send_byte(this->commandsToCode.find(msg.commandType)->second);
    this->send_byte(this->bools_to_code.find(msg.success)->second);
    if (msg.commandType == CommandType::CREATE_GAME) {
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
                if constexpr (std::is_same_v<T, GameInitialInfoDTO>) {
                    this->send_game_init_info(response);
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

void ServerProtocol::send_game_init_info(const GameInitialInfoDTO& dto) {
    this->send_byte(CODE_SEND_GAME_INIT_INFO);
    this->send_byte(static_cast<int>(dto.game_map.background));
    this->send_big_endian_number(dto.game_map.map_objects.size());
    for (auto object: dto.game_map.map_objects) {
        this->send_big_endian_number(object.type);
        this->send_byte(this->bools_to_code.find(object.collidable)->second);
        this->send_byte(object.positions.size());
        for (auto vec: object.positions) {
            this->send_big_endian_number(vec.x);
            this->send_big_endian_number(vec.y);
        }
    }
    this->send_byte(dto.shop_info.prices.size());
    for (const auto& [gun, price]: dto.shop_info.prices) {
        this->send_byte(this->weaponParser.getWeaponToByte(gun));
        this->send_big_endian_number(price);
    }
    this->send_byte(dto.shop_info.ammo_by_clip.size());
    for (const auto& [gun, price]: dto.shop_info.ammo_by_clip) {
        this->send_byte(this->weaponParser.getWeaponToByte(gun));
        this->send_big_endian_number(price);
    }
    this->send_byte(dto.shop_info.price_clips);
}


void ServerProtocol::send_snapshot(const Snapshot& snapshot) {
    this->send_byte(CODE_SNAPSHOT);
    this->send_byte(snapshot.total_players);
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
            return PlayerActionDTO{this->code_to_bools.find(this->receive_byte())->second};
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
    Movement movement = static_cast<Movement>(code_movement - 1);
    bool move = this->code_to_bools.find(this->receive_byte())->second;
    return MoveDTO{movement, move};
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
    bool primary = false;
    if (primary_code == CODE_CHOOSE_PRIMARY) {
        primary = true;
    }
    return BuyAmmoDTO{primary};
}


CreateUsernameDTO ServerProtocol::receive_create_username_request() {
    CreateUsernameDTO dto;
    std::string username = this->receive_string();
    dto.username = username;
    return dto;
}

CreateGameDTO ServerProtocol::receive_create_game_request() {
    return CreateGameDTO{this->receive_string()};
}

JoinGameDTO ServerProtocol::receive_join_game_request() {
    return JoinGameDTO{this->receive_string()};
}


RotateDTO ServerProtocol::receive_rotate() { return RotateDTO{this->receive_angle()}; }

void ServerProtocol::kill() {
    if (!this->socket->is_stream_recv_closed()) {
        this->socket->shutdown(SHUT_RD);
    }
    if (!this->socket->is_stream_send_closed()) {
        this->socket->shutdown(SHUT_WR);
    }
    this->socket->close();
}

ServerProtocol::~ServerProtocol() {}
