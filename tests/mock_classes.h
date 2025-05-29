#include <gmock/gmock.h>

#include "../src/client/client_protocol.h"
#include "../src/common/lobby_request.h"
#include "../src/common/message.h"
#include "../src/common/skins.h"


class MockClientProtocol: public ClientProtocol {
public:
    MOCK_METHOD(ServerResponseLobby, receive_command, (), (override));

    using ClientProtocol::send_lobby_request;

    MOCK_METHOD(void, send_create_username_request, (const CreateUsernameDTO& dto), (override));
    MOCK_METHOD(void, send_create_game_request, (const CreateGameDTO& dto), (override));
    MOCK_METHOD(void, send_join_game_request, (const JoinGameDTO& dto), (override));
};
