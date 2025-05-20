#include "server_protocol.h"

#include <cstring>
#include <utility>

#include <sys/types.h>

#include "../common/player_dto.h"
#include "../common/skins.h"

ServerProtocol::ServerProtocol(Socket&& socket):
        CommonProtocol(std::move(socket)),
        codeSuccessResponse({{true, CODE_SUCCESS}, {false, CODE_FAIL}}) {
    commandsManagers[CommandType::CREATE_USERNAME] = [this](const CommandType& command) {
        return receive_create_username_request(command);
    };
    commandsManagers[CommandType::CREATE_GAME] = [this](const CommandType& command) {
        return receive_create_game_request(command);
    };
    commandsManagers[CommandType::JOIN_GAME] = [this](const CommandType& command) {
        return receive_join_game_request(command);
    };
    commandsManagers[CommandType::SELECT_MAP] = [this](const CommandType& command) {
        return receive_select_map_request(command);
    };
    commandsManagers[CommandType::BUY_WEAPON] = [this](const CommandType& command) {
        return receive_buy_weapon_request(command);
    };
    commandsManagers[CommandType::BUY_AMMO] = [this](const CommandType& command) {
        return receive_buy_weapon_ammo_request(command);
    };
    commandsManagers[CommandType::AIM] = [this](const CommandType& command) {
        return receive_aim_request(command);
    };
    commandsManagers[CommandType::MOVE] = [this](const CommandType& command) {
        return receive_move_request(command);
    };
    commandsManagers[CommandType::SHOOT] = [this](const CommandType& command) {
        return receive_shoot_request(command);
    };
    commandsManagers[CommandType::CHANGE_WEAPON] = [this](const CommandType& command) {
        return receive_change_weapon_request(command);
    };
    commandsManagers[CommandType::PLANT_BOMB] = [this](const CommandType& command) {
        return receive_plant_bomb_request(command);
    };
    commandsManagers[CommandType::DEFUSE_BOMB] = [this](const CommandType& command) {
        return receive_defuse_bomb_request(command);
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

void ServerProtocol::send_snapshot(const Snapshot& snapshot) {
    // this->send_byte(snapshot.phase);
    // this->send_byte(snapshot.round_number);
    // this->send_byte(snapshot.bomb_status);
    // this->send_byte(snapshot.timer);
    this->send_byte(snapshot.players.size());
    this->send_players(snapshot.players);
    //  this->send_byte(snapshot.bullets.size());
    //  this->send_bullets(snapshot.bullets);
}

void ServerProtocol::send_players(const std::vector<PlayerDTO>& players) {
    for (auto player: players) {
        // this->send_string(player.username);
        this->send_byte(player.position.x);
        this->send_byte(player.position.y);
        this->send_byte(player.direction.x);
        this->send_byte(player.direction.y);
        this->send_byte(player.life);
        // this->send_big_endian_number(player.money);
        // this->send_byte(player.health);
        // this->send_byte(player.equipment.have_knife ? 0x01 : 0x00);
        // this->send_byte(this->weaponParser.getWeaponCode(player.equipment.primary_weapon));
        // this->send_big_endian_number(player.equipment.primary_weapon.bullets);
        // this->send_byte(this->weaponParser.getWeaponCode(player.equipment.secondary_weapon));
        // this->send_big_endian_number(player.equipment.secondary_weapon.bullets);
        // this->send_byte(player.equipment.have_bomb ? 0x01 : 0x00);
        // this->send_byte(player.is_shooting ? 0x01 : 0x00);
        // this->send_byte(player.weapon_equipped);
    }
}
/*
void ServerProtocol::send_bullets(const std::vector<Bullet>& bullets) {
    for (auto bullet : bullets) {
        this->send_byte(bullet.id);
        this->send_byte(bullet.pos_x);
        this->send_byte(bullet.pos_y);
        this->send_byte(bullet.dir_x);
        this->send_byte(bullet.dir_y);
    }
}
*/

MessageFromClient ServerProtocol::receive_command(void) {
    uint8_t commandCode = this->receive_byte();
    CommandType command = this->codeToCommands.find(commandCode)->second;
    return this->commandsManagers.find(command)->second(command);
}

MessageFromClient ServerProtocol::receive_create_username_request(const CommandType& command) {
    std::string username = this->receive_string();
    MessageFromClient msg = this->initialize_message(command);
    msg.s = username;
    return msg;
}

MessageFromClient ServerProtocol::initialize_message(const CommandType& command) {
    return MessageFromClient{command,
                             "",
                             GunType::NONE,
                             WeaponType::BOMB,
                             TerroristSkin::ARTIC_AVENGER,
                             CounterTerroristSkin::GIGN,
                             Movement::DOWN,
                             0,
                             0,
                             0,
                             0,
                             false};
}

MessageFromClient ServerProtocol::receive_create_game_request(const CommandType& command) {
    uint8_t size_players = this->receive_byte();
    MessageFromClient msg = this->receive_select_skins_request(command);
    msg.size_players = size_players;
    return msg;
}

MessageFromClient ServerProtocol::receive_join_game_request(const CommandType& command) {
    std::string gameName = this->receive_string();
    MessageFromClient msg = this->receive_select_skins_request(command);
    msg.commandType = command;
    msg.s = gameName;
    return msg;
}

MessageFromClient ServerProtocol::receive_select_skins_request(const CommandType& command) {
    uint8_t skin_id_tt = this->receive_byte();
    uint8_t skin_id_ct = this->receive_byte();
    MessageFromClient msg = this->initialize_message(command);
    msg.tt_skin = TerroristSkin(skin_id_tt - 1);
    msg.ct_skin = CounterTerroristSkin(skin_id_ct - 1);
    return msg;
}

MessageFromClient ServerProtocol::receive_select_map_request(const CommandType& command) {
    MessageFromClient msg = this->initialize_message(command);
    // msg.map_id = GameMap(this->receive_byte());
    // falta lo del enum o lo que fuere
    return msg;
}

MessageFromClient ServerProtocol::receive_buy_weapon_request(const CommandType& command) {
    GunType weapon = GunType(this->receive_byte());
    MessageFromClient msg = this->initialize_message(command);
    msg.weapon = weapon;
    return msg;
}

MessageFromClient ServerProtocol::receive_buy_weapon_ammo_request(const CommandType& command) {
    WeaponType weapon_type = WeaponType(this->receive_byte());
    uint16_t bullets = this->receive_big_endian_number();
    MessageFromClient msg = this->initialize_message(command);
    msg.weaponType = weapon_type;
    msg.bullets = bullets;
    return msg;
}

MessageFromClient ServerProtocol::receive_aim_request(const CommandType& command) {
    uint8_t pos_x = this->receive_byte();
    uint8_t pos_y = this->receive_byte();
    MessageFromClient msg = this->initialize_message(command);
    msg.pos_x = pos_x;
    msg.pos_y = pos_y;
    return msg;
}

MessageFromClient ServerProtocol::receive_move_request(const CommandType& command) {
    uint8_t direction = this->receive_byte();
    MessageFromClient msg = this->initialize_message(command);
    msg.movement = Movement(direction - 1);
    return msg;
}

MessageFromClient ServerProtocol::receive_shoot_request(const CommandType& command) {
    MessageFromClient msg = this->initialize_message(command);
    return msg;
}

MessageFromClient ServerProtocol::receive_change_weapon_request(const CommandType& command) {
    MessageFromClient msg = this->initialize_message(command);
    msg.weaponType = WeaponType(this->receive_byte());
    return msg;
}

MessageFromClient ServerProtocol::receive_plant_bomb_request(const CommandType& command) {
    MessageFromClient msg = this->initialize_message(command);
    return msg;
}

MessageFromClient ServerProtocol::receive_defuse_bomb_request(const CommandType& command) {
    MessageFromClient msg = this->initialize_message(command);
    return msg;
}

void ServerProtocol::send_map(const GameMap& map) {}

void ServerProtocol::kill() {
    this->socket.shutdown(SHUT_RDWR);
    this->socket.close();
}

ServerProtocol::~ServerProtocol() {}
