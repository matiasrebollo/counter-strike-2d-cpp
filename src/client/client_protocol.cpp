#include "client_protocol.h"

#include <cstdint>
#include <cstring>
#include <memory>
#include <string>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "../common/commands.h"
#include "../common/communication_ended.h"
#include "../common/message.h"
#include "../common/vector_2d.h"

ClientProtocol::ClientProtocol(std::unique_ptr<Socket> socket):
        CommonProtocol(std::move(socket)), isAlive(true) {}

ServerResponseLobby ClientProtocol::receive_server_response_lobby() {
    uint8_t code = this->receive_byte();
    ServerResponseLobby response = ServerResponseLobby{this->codeToCommands.find(code)->second,
                                                       ResponseStatus::SUCCESS, ""};
    if (this->codeToCommands.find(code)->second != CommandType::GAME_STARTED) {
        response.status = static_cast<ResponseStatus>(this->receive_byte());
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
                    static_assert(always_false_v<T>, "Unhandled GameCommandDTO type");
                }
            },
            request);
}

void ClientProtocol::send_command(const GameCommandDTO& command) {
    std::visit(
            [this](const auto& d) {
                using T = std::decay_t<decltype(d)>;
                if constexpr (std::is_same_v<T, MoveDTO>) {
                    handle_move(d);
                } else if constexpr (std::is_same_v<T, RotateDTO>) {
                    handle_rotate(d);
                } else if constexpr (std::is_same_v<T, PlayerActionDTO>) {
                    handle_player_action(d);
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
                    static_assert(always_false_v<T>, "Unhandled GameCommandDTO type");
                }
            },
            command);
}

void ClientProtocol::send_create_username_request(const CreateUsernameDTO& dto) {
    this->send_byte(this->commandsToCode.find(CommandType::CREATE_USERNAME)->second);
    this->send_string(dto.username);
}

void ClientProtocol::send_create_game_request(const CreateGameDTO& dto) {
    this->send_byte(commandsToCode.find(CommandType::CREATE_GAME)->second);
    this->send_string(dto.map_file_name);
}

void ClientProtocol::send_join_game_request(const JoinGameDTO& dto) {
    this->send_byte(this->commandsToCode.find(CommandType::JOIN_GAME)->second);
    this->send_string(dto.gamename);
}

void ClientProtocol::handle_move(const MoveDTO& dto) {
    this->send_byte(CODE_MOVE);
    this->send_byte(dto.dir + 1);
    this->send_byte(this->bools_to_code.find(dto.move)->second);
}

void ClientProtocol::handle_rotate(const RotateDTO& dto) {
    this->send_byte(CODE_ROTATE);
    this->send_angle(dto.angle);
}

void ClientProtocol::handle_player_action(const PlayerActionDTO& dto) {
    this->send_byte(CODE_ACTION);
    this->send_byte(this->bools_to_code.find(dto.make)->second);
}

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
}


GameDTO ClientProtocol::receive_game_dto() {
    uint8_t code = this->receive_byte();
    if (code == CODE_SEND_GAME_INIT_INFO) {
        return this->receive_game_initial_info();
    } else if (code == CODE_SNAPSHOT) {
        return this->receive_snapshot();
    } else {
        return GameEnded{};
    }
}

Snapshot ClientProtocol::receive_snapshot() {
    int total_players = this->receive_byte();
    int phase = this->receive_byte();
    size_t current_round_number = this->receive_byte();
    size_t total_rounds = this->receive_byte();
    int time_left = this->receive_byte();
    BombStatus status = static_cast<BombStatus>(this->receive_byte());
    std::optional<Vector2D<int>> bomb_position = this->receive_bomb_position();
    int size_ct = this->receive_byte();
    std::vector<PlayerDTO> cts = this->receive_players(size_ct);
    int size_tt = this->receive_byte();
    std::vector<PlayerDTO> tts = this->receive_players(size_tt);
    std::optional<Team> current_round_winner = this->receive_current_round_winner();
    Snapshot snapshot = Snapshot{total_players,       Phase(phase), current_round_number,
                                 total_rounds,        time_left,    status,
                                 bomb_position,       cts,          tts,
                                 current_round_winner};
    return snapshot;
}

std::vector<PlayerDTO> ClientProtocol::receive_players(const int& size_players) {
    std::vector<PlayerDTO> players = {};
    for (int i = 0; i < size_players; i++) {
        std::string username = this->receive_string();
        int position_x = this->receive_big_endian_number();
        int position_y = this->receive_big_endian_number();
        double angle = this->receive_angle();
        uint16_t life = this->receive_big_endian_number();
        std::optional<ShotDTO> shot = this->receive_shot();
        bool planting_bomb = this->code_to_bools.find(this->receive_byte())->second;
        bool on_site = this->code_to_bools.find(this->receive_byte())->second;
        int bonifications = this->receive_byte();
        int kills = this->receive_byte();
        int deaths = this->receive_byte();
        LoadoutDTO loadout = this->receive_loadout();
        players.push_back(PlayerDTO{username, Vector2D<int>(position_x, position_y), angle, life,
                                    shot, planting_bomb, on_site, bonifications, kills, deaths,
                                    loadout});
    }
    return players;
}

std::optional<ShotDTO> ClientProtocol::receive_shot() {
    bool has_value = this->code_to_bools.find(this->receive_byte())->second;
    double distance = this->receive_double();
    if (has_value) {
        return ShotDTO{distance};
    } else {
        return std::nullopt;
    }
}

std::optional<Vector2D<int>> ClientProtocol::receive_bomb_position() {
    bool has_value = this->code_to_bools.find(this->receive_byte())->second;
    int x = this->receive_byte();
    int y = this->receive_byte();
    if (has_value) {
        return Vector2D{x, y};
    } else {
        return std::nullopt;
    }
}

std::optional<Team> ClientProtocol::receive_current_round_winner() {
    bool has_value = this->code_to_bools.find(this->receive_byte())->second;
    Team team = static_cast<Team>(this->receive_byte());
    if (has_value) {
        return team;
    } else {
        return std::nullopt;
    }
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
    bool has_bomb = this->code_to_bools.find(this->receive_byte())->second;
    return LoadoutDTO{money,          primary_gun, primary_ammo, secondary_gun,
                      secondary_ammo, equipped,    has_bomb};
}

GameInitialInfoDTO ClientProtocol::receive_game_initial_info() {
    Background background = static_cast<Background>(this->receive_byte());
    uint16_t size = this->receive_big_endian_number();
    std::vector<MapObject> map_objects = this->receive_map_objects(size);
    uint16_t size_sites = this->receive_big_endian_number();
    std::set<Vector2D<int>> sites = this->receive_sites(size_sites);
    GameMapDTO game_map = GameMapDTO{background, map_objects, sites};
    uint8_t shop_gun_prices_size = this->receive_byte();
    std::unordered_map<GunType, int> gun_prices = this->receive_gun_prices(shop_gun_prices_size);
    uint8_t shop_gun_clips_size = this->receive_byte();
    std::unordered_map<GunType, int> gun_clips = this->receive_gun_clips_size(shop_gun_clips_size);
    int price_clips = this->receive_byte();
    ShopInfoDTO shop_info = ShopInfoDTO{gun_prices, gun_clips, price_clips};
    return GameInitialInfoDTO{game_map, shop_info};
}

std::set<Vector2D<int>> ClientProtocol::receive_sites(const uint16_t& size) {
    std::set<Vector2D<int>> sites = {};
    for (int i = 0; i < size; i++) {
        int x = static_cast<int>(this->receive_big_endian_number());
        int y = static_cast<int>(this->receive_big_endian_number());
        Vector2D<int> actual = Vector2D(x, y);
        sites.insert(actual);
    }
    return sites;
}


std::unordered_map<GunType, int> ClientProtocol::receive_gun_prices(const uint8_t& size) {
    std::unordered_map<GunType, int> response = {};
    for (int i = 0; i < size; i++) {
        GunType gun = this->weaponParser.getWeaponFromByte(this->receive_byte());
        int price_gun = this->receive_big_endian_number();
        response[gun] = price_gun;
    }
    return response;
}

std::unordered_map<GunType, int> ClientProtocol::receive_gun_clips_size(const uint8_t& size) {
    std::unordered_map<GunType, int> response = {};
    for (int i = 0; i < size; i++) {
        GunType gun = this->weaponParser.getWeaponFromByte(this->receive_byte());
        int clip_size = this->receive_big_endian_number();
        response[gun] = clip_size;
    }
    return response;
}


std::vector<MapObject> ClientProtocol::receive_map_objects(const uint8_t& size) {
    std::vector<MapObject> objects = {};
    for (int i = 0; i < size; i++) {
        uint16_t type = this->receive_big_endian_number();
        uint8_t collidable = this->receive_byte();
        uint8_t vec_size = this->receive_byte();
        std::vector<Vector2D<int>> positions;
        for (int j = 0; j < vec_size; j++) {
            uint16_t x = this->receive_big_endian_number();
            uint16_t y = this->receive_big_endian_number();
            positions.push_back(Vector2D<int>(x, y));
        }
        objects.push_back({positions, type, this->code_to_bools.find(collidable)->second});
    }
    return objects;
}

void ClientProtocol::close() {
    if (!this->socket->is_stream_recv_closed()) {
        this->socket->shutdown(SHUT_RD);
    }
    if (!this->socket->is_stream_send_closed()) {
        this->socket->shutdown(SHUT_WR);
    }
    this->socket->close();
}
