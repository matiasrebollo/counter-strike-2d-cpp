#include "mock_classes.h"

TEST(ClientProtocolTest, SendLobbyRequest_CallsCorrectCreateUsername) {
    MockClientProtocol mock;

    CreateUsernameDTO dto{"Taiel"};
    LobbyRequestDTO request = dto;

    EXPECT_CALL(mock, send_create_username_request(dto)).Times(1);

    mock.send_lobby_request(request);
}

TEST(ClientProtocolTest, SendLobbyRequest_CallsCorrectCreateGame) {
    MockClientProtocol mock;

    CreateGameDTO dto{4, CounterTerroristSkin::GIGN, TerroristSkin::ARTIC_AVENGER};
    LobbyRequestDTO request = dto;

    EXPECT_CALL(mock, send_create_game_request(dto)).Times(1);

    mock.send_lobby_request(request);
}

TEST(ClientProtocolTest, SendLobbyRequest_CallsCorrectJoinGame) {
    MockClientProtocol mock;

    JoinGameDTO dto{"partidaX", 3, 4};
    LobbyRequestDTO request = dto;

    EXPECT_CALL(mock, send_join_game_request(dto)).Times(1);

    mock.send_lobby_request(request);
}
