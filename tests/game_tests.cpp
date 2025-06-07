#include <gtest/gtest.h>

#include "common_tests.h"

/* CLIENT PROTOCOL REQUESTS */

TEST(ClientProtocolTest, SendMoveUp) {
    auto [client, server] = create_connected_protocols("10000");

    MoveUpDTO dto{};

    client->send_command(dto);

    CommandDTO request = server->receive_client_request();
    auto MoveUpDTOPtr = std::get_if<MoveUpDTO>(&request);
    ASSERT_NE(MoveUpDTOPtr, nullptr) << "Expected MoveUpDTO but got another";
}

TEST(ClientProtocolTest, SendMoveDown) {
    auto [client, server] = create_connected_protocols("10000");

    MoveDownDTO dto{};

    client->send_command(dto);

    CommandDTO request = server->receive_client_request();
    auto MoveDownDTOPtr = std::get_if<MoveDownDTO>(&request);
    ASSERT_NE(MoveDownDTOPtr, nullptr) << "Expected MoveDownDTO but got another";
}

TEST(ClientProtocolTest, SendMoveLeft) {
    auto [client, server] = create_connected_protocols("10000");

    MoveLeftDTO dto{};

    client->send_command(dto);

    CommandDTO request = server->receive_client_request();
    auto MoveLeftDTOPtr = std::get_if<MoveLeftDTO>(&request);
    ASSERT_NE(MoveLeftDTOPtr, nullptr) << "Expected MoveLeftDTO but got another";
}

TEST(ClientProtocolTest, SendMoveRight) {
    auto [client, server] = create_connected_protocols("10000");

    MoveRightDTO dto{};

    client->send_command(dto);

    CommandDTO request = server->receive_client_request();
    auto MoveRightDTOPtr = std::get_if<MoveRightDTO>(&request);
    ASSERT_NE(MoveRightDTOPtr, nullptr) << "Expected MoveRightDTO but got another";
}

TEST(ClientProtocolTest, SendRotate) {  // va haber error, pensar como hacer esto.
    auto [client, server] = create_connected_protocols("10000");

    std::vector<double> angles = {0, 50.24, 303.21313, 100.00};
    for (double angle: angles) {
        RotateDTO dto{angle};
        client->send_command(dto);

        CommandDTO request = server->receive_client_request();
        auto RotateDTOPtr = std::get_if<RotateDTO>(&request);
        ASSERT_NE(RotateDTOPtr, nullptr) << "Expected RotateDTO but got another";
        ASSERT_GE(RotateDTOPtr->angle, angle - 0.01);
        ASSERT_LE(RotateDTOPtr->angle, angle + 0.01);
    }
}

TEST(ClientProtocolTest, SendPlayerAction) {
    auto [client, server] = create_connected_protocols("10000");

    PlayerActionDTO dto{};

    client->send_command(dto);

    CommandDTO request = server->receive_client_request();
    auto PlayerActionDTOPtr = std::get_if<PlayerActionDTO>(&request);
    ASSERT_NE(PlayerActionDTOPtr, nullptr) << "Expected PlayerActionDTO but got another";
}

TEST(ClientProtocolTest, SendEquipPrimary) {
    auto [client, server] = create_connected_protocols("10000");

    EquipPrimaryDTO dto{};

    client->send_command(dto);

    CommandDTO request = server->receive_client_request();
    auto EquipPrimaryDTOPtr = std::get_if<EquipPrimaryDTO>(&request);
    ASSERT_NE(EquipPrimaryDTOPtr, nullptr) << "Expected EquipPrimaryDTO but got another";
}

TEST(ClientProtocolTest, SendEquipSecondary) {
    auto [client, server] = create_connected_protocols("10000");

    EquipSecondaryDTO dto{};

    client->send_command(dto);

    CommandDTO request = server->receive_client_request();
    auto EquipSecondaryDTOPtr = std::get_if<EquipSecondaryDTO>(&request);
    ASSERT_NE(EquipSecondaryDTOPtr, nullptr) << "Expected EquipSecondaryDTO but got another";
}

TEST(ClientProtocolTest, SendEquipKnife) {
    auto [client, server] = create_connected_protocols("10000");

    EquipKnifeDTO dto{};

    client->send_command(dto);

    CommandDTO request = server->receive_client_request();
    auto EquipKnifeDTOPtr = std::get_if<EquipKnifeDTO>(&request);
    ASSERT_NE(EquipKnifeDTOPtr, nullptr) << "Expected EquipKnifeDTO but got another";
}

TEST(ClientProtocolTest, SendEquipBomb) {
    auto [client, server] = create_connected_protocols("10000");

    EquipBombDTO dto{};

    client->send_command(dto);

    CommandDTO request = server->receive_client_request();
    auto EquipBombDTOPtr = std::get_if<EquipBombDTO>(&request);
    ASSERT_NE(EquipBombDTOPtr, nullptr) << "Expected EquipBombDTO but got another";
}

TEST(ClientProtocolTest, SendBuyGun) {
    auto [client, server] = create_connected_protocols("10000");

    for (int i = GLOCK; i <= AWP; ++i) {
        GunType gun = static_cast<GunType>(i);
        BuyGunDTO dto{gun};
        client->send_command(dto);

        CommandDTO request = server->receive_client_request();
        auto BuyGunDTOPtr = std::get_if<BuyGunDTO>(&request);
        ASSERT_NE(BuyGunDTOPtr, nullptr) << "Expected BuyGunDTO but got another";
        ASSERT_EQ(BuyGunDTOPtr->gun, gun);
    }
}

TEST(ClientProtocolTest, SendBuyAmmo) {
    auto [client, server] = create_connected_protocols("10000");

    std::vector<bool> for_primary_values = {false, true};

    for (int i = 0; i < 65536; ++i) {
        for (bool value: for_primary_values) {
            BuyAmmoDTO dto{static_cast<uint16_t>(i), value};
            client->send_command(dto);

            CommandDTO request = server->receive_client_request();
            auto BuyAmmoDTOPtr = std::get_if<BuyAmmoDTO>(&request);
            ASSERT_NE(BuyAmmoDTOPtr, nullptr) << "Expected BuyAmmoDTO but got another";
            ASSERT_EQ(BuyAmmoDTOPtr->ammo, static_cast<uint16_t>(i));
            ASSERT_EQ(BuyAmmoDTOPtr->for_primary, value);
        }
    }
}

/* SERVER PROTOCOL RESPONSES */

std::vector<LoadoutDTO> get_loadouts() {
    std::vector<LoadoutDTO> loadouts = {};
    std::vector<uint16_t> moneys = {1000, 30000};
    std::vector<GunType> guns = {GunType::AK47, GunType::AWP, GunType::M3};
    std::vector<uint16_t> ammos = {300, 400};
    std::vector<WeaponType> equippeds = {WeaponType::KNIFE, WeaponType::PRIMARY,
                                         WeaponType::SECONDARY, WeaponType::BOMB};
    for (auto money: moneys) {
        for (auto gun: guns) {
            for (auto ammo: ammos) {
                for (auto equipped: equippeds) {
                    // cppcheck-suppress useStlAlgorithm
                    loadouts.push_back(
                            LoadoutDTO{money, gun, ammo, GunType::GLOCK, ammo, equipped});
                }
            }
        }
    }
    return loadouts;
}

TEST(ServerProtocolTest, SendSnapshot) {
    auto [client, server] = create_connected_protocols("10000");

    std::vector<Phase> phases = {Phase::ATTACK, Phase::BUY};
    std::vector<size_t> current_rounds = {};
    for (int i = 0; i < 11; i++) {
        current_rounds.push_back(i);
    }

    size_t total_rounds = 10;

    std::vector<LoadoutDTO> loadouts = get_loadouts();

    for (auto phase: phases) {
        for (auto current_round: current_rounds) {
            for (auto loadout: loadouts) {
                std::vector<PlayerDTO> ct = {PlayerDTO{"Mati", Vector2D(0, 0), 0, 100, loadout}};
                std::vector<PlayerDTO> tt = {
                        PlayerDTO{"Facu", Vector2D(10, 10), 100, 100, loadout}};
                Snapshot snapshot{phase, current_round, total_rounds, 20, ct, tt};

                server->send_game_dto(snapshot);
                GameDTO response = client->receive_game_dto();
                auto snapshotPTr = std::get_if<Snapshot>(&response);
                ASSERT_NE(snapshotPTr, nullptr) << "Expected SnapshotDTO but got another";
                ASSERT_EQ(snapshotPTr->phase, phase);
                ASSERT_EQ(snapshotPTr->current_round_number, current_round);
                ASSERT_EQ(snapshotPTr->total_rounds, total_rounds);
                ASSERT_EQ(snapshotPTr->time_left, 20);
                ASSERT_EQ(snapshotPTr->ct[0].life, 100);
                ASSERT_EQ(snapshotPTr->tt[0].life, 100);
                ASSERT_EQ(snapshotPTr->ct[0].loadout.primary_gun, loadout.primary_gun);
                ASSERT_EQ(snapshotPTr->tt[0].loadout.primary_gun, loadout.primary_gun);
                ASSERT_EQ(snapshotPTr->ct[0].loadout.secondary_gun, loadout.secondary_gun);
                ASSERT_EQ(snapshotPTr->tt[0].loadout.secondary_gun, loadout.secondary_gun);
                ASSERT_EQ(snapshotPTr->ct[0].loadout.primary_ammo, loadout.primary_ammo);
                ASSERT_EQ(snapshotPTr->tt[0].loadout.primary_ammo, loadout.primary_ammo);
                ASSERT_EQ(snapshotPTr->ct[0].loadout.secondary_ammo, loadout.secondary_ammo);
                ASSERT_EQ(snapshotPTr->tt[0].loadout.secondary_ammo, loadout.secondary_ammo);
                ASSERT_EQ(snapshotPTr->ct[0].loadout.equipped, loadout.equipped);
                ASSERT_EQ(snapshotPTr->tt[0].loadout.equipped, loadout.equipped);
            }
        }
    }
}
