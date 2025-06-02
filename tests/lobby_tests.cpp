#include <gtest/gtest.h>

#include "../src/client/client_protocol.h"
#include "../src/common/socket.h"
#include "../src/server/server_protocol.h"

/* LOBBY TESTS */

/* CLIENT REQUESTS TESTS */

TEST(ClientProtocolTest, SendCreateUsername) {
    Socket listener("10000");
    ClientProtocol client("localhost", "10000");
    Socket accepted_skt = listener.accept();

    ServerProtocol server(std::move(accepted_skt));

    std::string username = "Taiel";

    CreateUsernameDTO dto{username};

    client.send_lobby_request(dto);

    LobbyRequestDTO request = server.receive_lobby_request();

    auto createUsernamePtr = std::get_if<CreateUsernameDTO>(&request);
    ASSERT_NE(createUsernamePtr, nullptr) << "Expected CreateUsernameDTO but got another";
    ASSERT_EQ(createUsernamePtr->username, username);
}

TEST(ClientProtocolTest, SendCreateGame) {
    Socket listener("10000");
    ClientProtocol client("localhost", "10000");
    Socket accepted_skt = listener.accept();

    ServerProtocol server(std::move(accepted_skt));

    CreateGameDTO dto{};

    client.send_lobby_request(dto);

    LobbyRequestDTO request = server.receive_lobby_request();
    auto createGamePtr = std::get_if<CreateGameDTO>(&request);
    ASSERT_NE(createGamePtr, nullptr) << "Expected CreateGameDTO but got another";
}

TEST(ClientProtocolTest, SendJoinGame) {
    Socket listener("10000");
    ClientProtocol client("localhost", "10000");
    Socket accepted_skt = listener.accept();

    ServerProtocol server(std::move(accepted_skt));

    std::string gamename = "mygame";

    JoinGameDTO dto{gamename};

    client.send_lobby_request(dto);

    LobbyRequestDTO request = server.receive_lobby_request();

    auto joinGamePtr = std::get_if<JoinGameDTO>(&request);
    ASSERT_NE(joinGamePtr, nullptr) << "Expected JoinGameDTO but got another";
    ASSERT_EQ(joinGamePtr->gamename, gamename);
}

/* SERVER PROTOCOL RESPONSES */

TEST(ServerProtocolTest, SendLobbyResponse) {
    Socket listener("10000");
    ClientProtocol client("localhost", "10000");
    Socket accepted_skt = listener.accept();

    ServerProtocol server(std::move(accepted_skt));

    std::vector<CommandType> commands = {CommandType::CREATE_USERNAME, CommandType::CREATE_GAME,
                                         CommandType::JOIN_GAME};
    std::vector<bool> success_values = {false, true};
    std::vector<std::string> gamenames = {
            "", "mipartida"
                "unnombresuperlargoquequieroponeramipartidaporquesoyunserverloco"};

    for (auto command: commands) {
        for (auto value: success_values) {
            for (auto name: gamenames) {
                ServerResponseLobby dto{command, value, name};

                server.send_lobby_message(dto);

                ServerResponseLobby response = client.receive_command();

                ASSERT_EQ(response.commandType, command);
                ASSERT_EQ(response.success, value);
                // Only if the client creates a game the server reply with a name game's name. If
                // it's another command, it doesn't send anything so the game_name in the dto is
                // empty.
                if (command == CommandType::CREATE_GAME) {
                    ASSERT_EQ(response.game_name, name);
                } else {
                    ASSERT_EQ(response.game_name, "");
                }
            }
        }
    }
}
