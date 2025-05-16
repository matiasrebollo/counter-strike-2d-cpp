#include "client_protocol_parser.h"

ClientProtocolParser::ClientProtocolParser() {
    parsersMap[CommandType::CREATE_USERNAME] = [this](const MessageFromClient& request) {
        return this->parseFromCreateUsernameRequest(request);
    };
    parsersMap[CommandType::CREATE_GAME] = [this](const MessageFromClient& request) {
        return this->parseFromCreateGameRequest(request);
    };
    parsersMap[CommandType::JOIN_GAME] = [this](const MessageFromClient& request) {
        return this->parseFromJoinGameRequest(request);
    };
    parsersMap[CommandType::SELECT_MAP] = [this](const MessageFromClient& request) {
        return this->parseFromSelectMapRequest(request);
    };
    parsersMap[CommandType::BUY_WEAPON] = [this](const MessageFromClient& request) {
        return this->parseFromBuyWeaponRequest(request);
    };
    parsersMap[CommandType::BUY_AMMO] = [this](const MessageFromClient& request) {
        return this->parseFromBuyWeaponAmmoRequest(request);
    };
    parsersMap[CommandType::AIM] = [this](const MessageFromClient& request) {
        return this->parseFromAimRequest(request);
    };
    parsersMap[CommandType::MOVE] = [this](const MessageFromClient& request) {
        return this->parseFromMoveRequest(request);
    };
    parsersMap[CommandType::SHOOT] = [this](const MessageFromClient& request) {
        return this->parseFromShootRequest(request);
    };
    parsersMap[CommandType::PLANT_BOMB] = [this](const MessageFromClient& request) {
        return this->parseFromPlantBombRequest(request);
    };
    parsersMap[CommandType::DEFUSE_BOMB] = [this](const MessageFromClient& request) {
        return this->parseFromDefuseBombRequest(request);
    };
}

InternalMessage ClientProtocolParser::ParseMessage(const MessageFromClient& message) {
    return this->parsersMap.find(message.commandType)->second(message);
}

InternalMessage ClientProtocolParser::parseFromCreateUsernameRequest(const MessageFromClient& request) {
    InternalMessage msg = this->getInternalMessageWCode(request);
    msg.s = request.s;
    return msg;
}

InternalMessage ClientProtocolParser::parseFromCreateGameRequest(const MessageFromClient& request) {
    InternalMessage msg = this->getInternalMessageWCode(request);
    msg.size_players = request.size_players;
    this->parseSkins(request, msg);
    return msg;
}

InternalMessage ClientProtocolParser::parseFromJoinGameRequest(const MessageFromClient& request) {
    InternalMessage msg = this->getInternalMessageWCode(request);
    msg.s = request.s;
    this->parseSkins(request, msg);
    return msg;
}

void ClientProtocolParser::parseSkins(const MessageFromClient& request, InternalMessage& msg) {
    msg.skin_id_tt = request.tt_skin + 1;
    msg.skin_id_ct = request.ct_skin + 1;
}

InternalMessage ClientProtocolParser::parseFromSelectMapRequest(const MessageFromClient& request) {
    return this->getInternalMessageWCode(request);
}

InternalMessage ClientProtocolParser::parseFromBuyWeaponRequest(const MessageFromClient& request) {
    InternalMessage msg = this->getInternalMessageWCode(request);
    msg.code_weapon = this->weaponParser.getWeaponToByte(request.weapon);
    return msg;
}

InternalMessage ClientProtocolParser::parseFromBuyWeaponAmmoRequest(const MessageFromClient& request) {
    InternalMessage msg = this->getInternalMessageWCode(request);
    msg.code_weapon_type = this->weaponParser.getWeaponTypeToByte(this->weaponParser.getWeaponType(request.weapon));
    msg.bullets = request.bullets;
    return msg;
}

InternalMessage ClientProtocolParser::parseFromAimRequest(const MessageFromClient& request) {
    InternalMessage msg = this->getInternalMessageWCode(request);
    msg.pos_x = request.pos_x;
    msg.pos_y = request.pos_y;
    return msg;
}

InternalMessage ClientProtocolParser::parseFromMoveRequest(const MessageFromClient& request) {
    InternalMessage msg = this->getInternalMessageWCode(request);
    msg.movement = request.movement + 1;
    return msg;
}

InternalMessage ClientProtocolParser::parseFromShootRequest(const MessageFromClient& request) {
    return this->getInternalMessageWCode(request);
}

InternalMessage ClientProtocolParser::parseFromChangeWeaponRequest(const MessageFromClient& request) {
    InternalMessage msg = this->getInternalMessageWCode(request);
    msg.code_weapon_type = request.weaponType;
    return msg;
}

InternalMessage ClientProtocolParser::parseFromPlantBombRequest(const MessageFromClient& request) {
    return this->getInternalMessageWCode(request);
}

InternalMessage ClientProtocolParser::parseFromDefuseBombRequest(const MessageFromClient& request) {
    return this->getInternalMessageWCode(request);
}

InternalMessage ClientProtocolParser::getInternalMessageWCode(const MessageFromClient& request) {
    return InternalMessage{this->commandsToCode.find(request.commandType)->second};
}