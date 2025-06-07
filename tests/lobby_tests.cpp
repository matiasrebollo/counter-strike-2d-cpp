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

TEST(ClientProtocolTest, SendMoveUp) {
    Socket listener("10000");
    ClientProtocol client("localhost", "10000");
    Socket accepted_skt = listener.accept();

    ServerProtocol server(std::move(accepted_skt));

    MoveUpDTO dto{};

    client.send_command(dto);

    CommandDTO request = server.receive_client_request();
    auto createGamePtr = std::get_if<MoveUpDTO>(&request);
    ASSERT_NE(createGamePtr, nullptr) << "Expected MoveUpDTO but got another";
}

TEST(ClientProtocolTest, SendMoveDown) {
    Socket listener("10000");
    ClientProtocol client("localhost", "10000");
    Socket accepted_skt = listener.accept();

    ServerProtocol server(std::move(accepted_skt));

    MoveDownDTO dto{};

    client.send_command(dto);

    CommandDTO request = server.receive_client_request();
    auto createGamePtr = std::get_if<MoveDownDTO>(&request);
    ASSERT_NE(createGamePtr, nullptr) << "Expected MoveDownDTO but got another";
}

TEST(ClientProtocolTest, SendMoveLeft) {
    Socket listener("10000");
    ClientProtocol client("localhost", "10000");
    Socket accepted_skt = listener.accept();

    ServerProtocol server(std::move(accepted_skt));

    MoveLeftDTO dto{};

    client.send_command(dto);

    CommandDTO request = server.receive_client_request();
    auto createGamePtr = std::get_if<MoveLeftDTO>(&request);
    ASSERT_NE(createGamePtr, nullptr) << "Expected MoveLeftDTO but got another";
}

TEST(ClientProtocolTest, SendMoveRight) {
    Socket listener("10000");
    ClientProtocol client("localhost", "10000");
    Socket accepted_skt = listener.accept();

    ServerProtocol server(std::move(accepted_skt));

    MoveRightDTO dto{};

    client.send_command(dto);

    CommandDTO request = server.receive_client_request();
    auto createGamePtr = std::get_if<MoveRightDTO>(&request);
    ASSERT_NE(createGamePtr, nullptr) << "Expected MoveRightDTO but got another";
}

TEST(ClientProtocolTest, SendRotate) {  // va haber error, pensar como hacer esto.
    Socket listener("10000");
    ClientProtocol client("localhost", "10000");
    Socket accepted_skt = listener.accept();

    ServerProtocol server(std::move(accepted_skt));
}

TEST(ClientProtocolTest, SendPlayerAction) {
    Socket listener("10000");
    ClientProtocol client("localhost", "10000");
    Socket accepted_skt = listener.accept();

    ServerProtocol server(std::move(accepted_skt));

    PlayerActionDTO dto{};

    client.send_command(dto);

    CommandDTO request = server.receive_client_request();
    auto createGamePtr = std::get_if<PlayerActionDTO>(&request);
    ASSERT_NE(createGamePtr, nullptr) << "Expected PlayerActionDTO but got another";
}

TEST(ClientProtocolTest, SendEquipPrimary) {
    Socket listener("10000");
    ClientProtocol client("localhost", "10000");
    Socket accepted_skt = listener.accept();

    ServerProtocol server(std::move(accepted_skt));

    EquipPrimaryDTO dto{};

    client.send_command(dto);

    CommandDTO request = server.receive_client_request();
    auto createGamePtr = std::get_if<EquipPrimaryDTO>(&request);
    ASSERT_NE(createGamePtr, nullptr) << "Expected EquipPrimaryDTO but got another";
}

TEST(ClientProtocolTest, SendEquipSecondary) {
    Socket listener("10000");
    ClientProtocol client("localhost", "10000");
    Socket accepted_skt = listener.accept();

    ServerProtocol server(std::move(accepted_skt));

    EquipSecondaryDTO dto{};

    client.send_command(dto);

    CommandDTO request = server.receive_client_request();
    auto createGamePtr = std::get_if<EquipSecondaryDTO>(&request);
    ASSERT_NE(createGamePtr, nullptr) << "Expected EquipSecondaryDTO but got another";
}

TEST(ClientProtocolTest, SendEquipKnife) {
    Socket listener("10000");
    ClientProtocol client("localhost", "10000");
    Socket accepted_skt = listener.accept();

    ServerProtocol server(std::move(accepted_skt));

    EquipKnifeDTO dto{};

    client.send_command(dto);

    CommandDTO request = server.receive_client_request();
    auto createGamePtr = std::get_if<EquipKnifeDTO>(&request);
    ASSERT_NE(createGamePtr, nullptr) << "Expected EquipKnifeDTO but got another";
}

TEST(ClientProtocolTest, SendEquipBomb) {
    Socket listener("10000");
    ClientProtocol client("localhost", "10000");
    Socket accepted_skt = listener.accept();

    ServerProtocol server(std::move(accepted_skt));

    EquipBombDTO dto{};

    client.send_command(dto);

    CommandDTO request = server.receive_client_request();
    auto createGamePtr = std::get_if<EquipBombDTO>(&request);
    ASSERT_NE(createGamePtr, nullptr) << "Expected EquipBombDTO but got another";
}

TEST(ClientProtocolTest, SendBuyGun) {
    Socket listener("10000");
    ClientProtocol client("localhost", "10000");
    Socket accepted_skt = listener.accept();

    ServerProtocol server(std::move(accepted_skt));

    for (int i = GLOCK; i <= AWP; ++i) {
        GunType gun = static_cast<GunType>(i);
        BuyGunDTO dto{gun};
        client.send_command(dto);

        CommandDTO request = server.receive_client_request();
        auto buyGunPtr = std::get_if<BuyGunDTO>(&request);
        ASSERT_NE(buyGunPtr, nullptr) << "Expected BuyGunDTO but got another";
        ASSERT_EQ(buyGunPtr->gun, gun);
    }
}

TEST(ClientProtocolTest, SendBuyAmmo) {
    Socket listener("10000");
    ClientProtocol client("localhost", "10000");
    Socket accepted_skt = listener.accept();

    ServerProtocol server(std::move(accepted_skt));

    std::vector<bool> for_primary_values = {false, true};

    for (int i = 0; i < 65536; ++i) {
        for (bool value: for_primary_values) {
            BuyAmmoDTO dto{static_cast<uint16_t>(i), value};
            client.send_command(dto);

            CommandDTO request = server.receive_client_request();
            auto buyAmmoPtr = std::get_if<BuyAmmoDTO>(&request);
            ASSERT_NE(buyAmmoPtr, nullptr) << "Expected BuyAmmoDTO but got another";
            ASSERT_EQ(buyAmmoPtr->ammo, static_cast<uint16_t>(i));
            ASSERT_EQ(buyAmmoPtr->for_primary, value);
        }
    }
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
