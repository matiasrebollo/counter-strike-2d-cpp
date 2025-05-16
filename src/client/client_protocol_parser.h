#ifndef CLIENT_PROTOCOL_PARSER_H
#define CLIENT_PROTOCOL_PARSER_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../common/commands.h"
#include "../common/message.h"
#include "../common/weaponParser.h"
#include "../common/codes_parser.h"

class ClientProtocolParser : public CodesParser {
private:
    WeaponParser weaponParser;
    std::unordered_map<CommandType,
                       std::function<InternalMessage(const MessageFromClient& request)>>
            parsersMap;

    InternalMessage parseFromCreateUsernameRequest(const MessageFromClient& request);
    InternalMessage parseFromCreateGameRequest(const MessageFromClient& request);
    InternalMessage parseFromSelectMapRequest(const MessageFromClient& request);
    InternalMessage parseFromJoinGameRequest(const MessageFromClient& request);
    InternalMessage parseFromBuyWeaponRequest(const MessageFromClient& request);
    InternalMessage parseFromBuyWeaponAmmoRequest(const MessageFromClient& request);
    InternalMessage parseFromAimRequest(const MessageFromClient& request);
    InternalMessage parseFromMoveRequest(const MessageFromClient& request);
    InternalMessage parseFromShootRequest(const MessageFromClient& request);
    InternalMessage parseFromChangeWeaponRequest(const MessageFromClient& request);
    InternalMessage parseFromPlantBombRequest(const MessageFromClient& request);
    InternalMessage parseFromDefuseBombRequest(const MessageFromClient& request);

    void parseSkins(const MessageFromClient& request, InternalMessage& msg);
    
    InternalMessage getInternalMessageWCode(const MessageFromClient& request);

public:
    ClientProtocolParser();
    InternalMessage ParseMessage(const MessageFromClient& message);
    uint8_t ParseCoordinatesForSend(const std::vector<int>& coordinates);
};

#endif
