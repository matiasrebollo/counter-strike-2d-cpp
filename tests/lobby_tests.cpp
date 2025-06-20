#include <gtest/gtest.h>

#include "common_tests.h"

/* LOBBY TESTS */

/* CLIENT REQUESTS TESTS */

TEST(ClientProtocolTest, SendCreateUsername) {
    auto [client, server] = create_connected_protocols();

    std::string username = "Taiel";

    CreateUsernameDTO dto{username};

    client->send_lobby_request(dto);

    LobbyRequestDTO request = server->receive_lobby_request();

    auto createUsernamePtr = std::get_if<CreateUsernameDTO>(&request);
    ASSERT_NE(createUsernamePtr, nullptr) << "Expected CreateUsernameDTO but got another";
    ASSERT_EQ(createUsernamePtr->username, username);
}

TEST(ClientProtocolTest, SendCreateGame) {
    auto [client, server] = create_connected_protocols();

    CreateGameDTO dto{};

    client->send_lobby_request(dto);

    LobbyRequestDTO request = server->receive_lobby_request();
    auto createGamePtr = std::get_if<CreateGameDTO>(&request);
    ASSERT_NE(createGamePtr, nullptr) << "Expected CreateGameDTO but got another";
}

TEST(ClientProtocolTest, SendJoinGame) {
    auto [client, server] = create_connected_protocols();

    std::string gamename = "mygame";

    JoinGameDTO dto{gamename};

    client->send_lobby_request(dto);

    LobbyRequestDTO request = server->receive_lobby_request();

    auto joinGamePtr = std::get_if<JoinGameDTO>(&request);
    ASSERT_NE(joinGamePtr, nullptr) << "Expected JoinGameDTO but got another";
    ASSERT_EQ(joinGamePtr->gamename, gamename);
}

/* SERVER PROTOCOL RESPONSES */

TEST(ServerProtocolTest, SendLobbyResponse) {
    auto [client, server] = create_connected_protocols();

    std::vector<CommandType> commands = {CommandType::CREATE_USERNAME, CommandType::CREATE_GAME,
                                         CommandType::JOIN_GAME};
    std::vector<ResponseStatus> success_values = {
            ResponseStatus::SUCCESS,         ResponseStatus::GAME_NOT_CREATED,
            ResponseStatus::USERNAME_IN_USE, ResponseStatus::GAME_NOT_EXIST,
            ResponseStatus::GAME_IS_FULL,    ResponseStatus::USERNAME_ALREADY_IN_GAME,
            ResponseStatus::WITHOUT_USERNAME};
    std::vector<std::string> gamenames = {"", "mipartida"
                                              "unnombresuperlargoquequieroponera"};

    for (auto command: commands) {
        for (auto value: success_values) {
            for (auto name: gamenames) {
                if (command == CommandType::CREATE_GAME && name == "") {
                    // This case will never happen bc the server MUST create a valid name,
                    // in our case are numbers mapped to_string.
                    continue;
                }
                ServerResponseLobby dto{command, value, name};

                server->send_lobby_message(dto);

                ServerResponseLobby response = client->receive_server_response_lobby();

                ASSERT_EQ(response.commandType, command);
                ASSERT_EQ(response.status, value);
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
