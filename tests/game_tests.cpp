#include <gtest/gtest.h>

#include "common_tests.h"

/* CLIENT PROTOCOL REQUESTS */

TEST(ClientProtocolTest, SendMoveUp) {
    auto [client, server] = create_connected_protocols();


    std::vector<Movement> movements = {Movement::UP, Movement::DOWN, Movement::LEFT,
                                       Movement::RIGHT};
    std::vector<std::string> movements_string = {"Movement::UP", "Movement::DOWN", "Movement::LEFT",
                                                 "Movement::RIGHT"};

    std::vector<bool> values = {false, true};

    for (size_t i = 0; i < movements.size(); i++) {
        for (size_t j = 0; j < values.size(); j++) {

            MoveDTO dto = MoveDTO{movements[i], values[j]};

            client->send_command(dto);
            CommandDTO request = server->receive_client_request();
            auto MoveDTOPtr = std::get_if<MoveDTO>(&request);
            ASSERT_NE(MoveDTOPtr, nullptr) << "Expected MoveDTO but got another";
            ASSERT_EQ(movements[i], MoveDTOPtr->dir)
                    << "Expected " << movements_string[i] << " but got another";
            ASSERT_EQ(values[j], MoveDTOPtr->move)
                    << "Expected " << values[j] << " but got another";
        }
    }
}

TEST(ClientProtocolTest, SendRotate) {
    auto [client, server] = create_connected_protocols();

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
    auto [client, server] = create_connected_protocols();


    std::vector<bool> values = {false, true};

    for (bool v: values) {
        PlayerActionDTO dto{v};
        client->send_command(dto);

        CommandDTO request = server->receive_client_request();
        auto PlayerActionDTOPtr = std::get_if<PlayerActionDTO>(&request);
        ASSERT_NE(PlayerActionDTOPtr, nullptr) << "Expected PlayerActionDTO but got another";
        ASSERT_EQ(v, PlayerActionDTOPtr->make);
    }
}

TEST(ClientProtocolTest, SendEquipPrimary) {
    auto [client, server] = create_connected_protocols();

    EquipPrimaryDTO dto{};

    client->send_command(dto);

    CommandDTO request = server->receive_client_request();
    auto EquipPrimaryDTOPtr = std::get_if<EquipPrimaryDTO>(&request);
    ASSERT_NE(EquipPrimaryDTOPtr, nullptr) << "Expected EquipPrimaryDTO but got another";
}

TEST(ClientProtocolTest, SendEquipSecondary) {
    auto [client, server] = create_connected_protocols();

    EquipSecondaryDTO dto{};

    client->send_command(dto);

    CommandDTO request = server->receive_client_request();
    auto EquipSecondaryDTOPtr = std::get_if<EquipSecondaryDTO>(&request);
    ASSERT_NE(EquipSecondaryDTOPtr, nullptr) << "Expected EquipSecondaryDTO but got another";
}

TEST(ClientProtocolTest, SendEquipKnife) {
    auto [client, server] = create_connected_protocols();

    EquipKnifeDTO dto{};

    client->send_command(dto);

    CommandDTO request = server->receive_client_request();
    auto EquipKnifeDTOPtr = std::get_if<EquipKnifeDTO>(&request);
    ASSERT_NE(EquipKnifeDTOPtr, nullptr) << "Expected EquipKnifeDTO but got another";
}

TEST(ClientProtocolTest, SendEquipBomb) {
    auto [client, server] = create_connected_protocols();

    EquipBombDTO dto{};

    client->send_command(dto);

    CommandDTO request = server->receive_client_request();
    auto EquipBombDTOPtr = std::get_if<EquipBombDTO>(&request);
    ASSERT_NE(EquipBombDTOPtr, nullptr) << "Expected EquipBombDTO but got another";
}

TEST(ClientProtocolTest, SendBuyGun) {
    auto [client, server] = create_connected_protocols();

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
    auto [client, server] = create_connected_protocols();

    std::vector<bool> for_primary_values = {false, true};

    // for (int i = 0; i < 65536; ++i) {
    for (bool value: for_primary_values) {
        BuyAmmoDTO dto{/*static_cast<uint16_t>(i),*/ value};
        client->send_command(dto);

        CommandDTO request = server->receive_client_request();
        auto BuyAmmoDTOPtr = std::get_if<BuyAmmoDTO>(&request);
        ASSERT_NE(BuyAmmoDTOPtr, nullptr) << "Expected BuyAmmoDTO but got another";
        // ASSERT_EQ(BuyAmmoDTOPtr->ammo, static_cast<uint16_t>(i));
        ASSERT_EQ(BuyAmmoDTOPtr->for_primary, value);
    }
    // }
}

/* SERVER PROTOCOL RESPONSES */

void validate_map(const GameMapDTO& expected_gamemap, const GameMapDTO& actual_gamemap) {
    ASSERT_EQ(expected_gamemap.background, actual_gamemap.background);
    ASSERT_EQ(expected_gamemap.map_objects.size(), actual_gamemap.map_objects.size());
    for (size_t i = 0; i < expected_gamemap.map_objects.size(); i++) {
        ASSERT_EQ(expected_gamemap.map_objects[i].collidable,
                  actual_gamemap.map_objects[i].collidable);
        ASSERT_EQ(expected_gamemap.map_objects[i].type, actual_gamemap.map_objects[i].type);
        for (size_t j = 0; j < expected_gamemap.map_objects[i].positions.size(); j++) {
            ASSERT_EQ(expected_gamemap.map_objects[i].positions[j],
                      actual_gamemap.map_objects[i].positions[j]);
        }
    }
}

void validate_shop_info(const ShopInfoDTO& expected_shop_info,
                        const ShopInfoDTO& actual_shop_info) {
    ASSERT_EQ(expected_shop_info.prices.size(), actual_shop_info.prices.size());
    ASSERT_EQ(expected_shop_info.ammo_by_clip.size(), actual_shop_info.ammo_by_clip.size());
    for (const auto& [gun, price]: expected_shop_info.prices) {
        EXPECT_NE(actual_shop_info.prices.find(gun), actual_shop_info.prices.end());
        ASSERT_EQ(price, actual_shop_info.prices.find(gun)->second);
    }
    for (const auto& [gun, price]: expected_shop_info.ammo_by_clip) {
        EXPECT_NE(actual_shop_info.ammo_by_clip.find(gun), actual_shop_info.prices.end());
        ASSERT_EQ(price, actual_shop_info.ammo_by_clip.find(gun)->second);
    }
}

TEST(ServerProtocolTest, SendGameInitialInfo) {
    auto [client, server] = create_connected_protocols();

    std::vector<MapObject> objects = {};

    for (int i = 0; i < 100; i++) {
        std::vector<Vector2D<int>> positions = {Vector2D(0, 1), Vector2D(0, 2), Vector2D(0, 3)};
        MapObject object = {positions, 19, true};
        objects.push_back(object);
    }

    GameMapDTO game_map = GameMapDTO{Background::AZTEC_BACKGROUND, objects};

    std::unordered_map<GunType, int> prices = {
            {GunType::AK47, 2700},
            {GunType::M3, 3000},
            {GunType::AWP, 4750},
    };

    std::unordered_map<GunType, int> ammo_by_clip = {
            {GunType::GLOCK, 30},
            {GunType::AK47, 25},
            {GunType::M3, 8},
            {GunType::AWP, 4},
    };

    ShopInfoDTO shop_info = ShopInfoDTO{prices, ammo_by_clip};

    GameInitialInfoDTO dto = GameInitialInfoDTO{game_map, shop_info};

    server->send_game_dto(dto);

    GameDTO response = client->receive_game_dto();

    auto game_map_ptr = std::get_if<GameInitialInfoDTO>(&response);
    ASSERT_NE(game_map_ptr, nullptr) << "Expected GameInitialInfoDTO but got another";
    validate_map(dto.game_map, game_map_ptr->game_map);
}

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

void validate_player(const PlayerDTO& expected_player, const PlayerDTO& actual_player) {
    ASSERT_EQ(expected_player.username, actual_player.username);
    ASSERT_EQ(expected_player.life, actual_player.life);
    ASSERT_EQ(expected_player.bonifications, actual_player.bonifications);
    ASSERT_EQ(expected_player.kills, actual_player.kills);
    ASSERT_EQ(expected_player.deaths, actual_player.deaths);
    ASSERT_EQ(expected_player.loadout.primary_gun, actual_player.loadout.primary_gun);
    ASSERT_EQ(expected_player.loadout.secondary_gun, actual_player.loadout.secondary_gun);
    ASSERT_EQ(expected_player.loadout.primary_ammo, actual_player.loadout.primary_ammo);
    ASSERT_EQ(expected_player.loadout.secondary_ammo, actual_player.loadout.secondary_ammo);
    ASSERT_EQ(expected_player.loadout.equipped, actual_player.loadout.equipped);
    ASSERT_EQ(expected_player.shot.has_value(), actual_player.shot.has_value());
    if (expected_player.shot.has_value()) {
        ASSERT_LE(expected_player.shot->distance, actual_player.shot->distance - 0.1);
        ASSERT_GE(expected_player.shot->distance, actual_player.shot->distance + 0.1);
    }
}

TEST(ServerProtocolTest, SendSnapshot) {
    auto [client, server] = create_connected_protocols();

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
                std::vector<PlayerDTO> ct = {
                        PlayerDTO{"Mati", Vector2D(0, 0), 0, 100, std::nullopt, 0, 0, 0, loadout}};
                std::vector<PlayerDTO> tt = {PlayerDTO{"Facu", Vector2D(10, 10), 100, 100,
                                                       std::optional<ShotDTO>(100.20), 10, 10, 10,
                                                       loadout}};
                Snapshot snapshot{2, phase, current_round, total_rounds, 20, ct, tt};

                server->send_game_dto(snapshot);
                GameDTO response = client->receive_game_dto();
                auto snapshotPtr = std::get_if<Snapshot>(&response);
                ASSERT_NE(snapshotPtr, nullptr) << "Expected SnapshotDTO but got another";
                ASSERT_EQ(snapshotPtr->phase, phase);
                ASSERT_EQ(snapshotPtr->total_players, 2);
                ASSERT_EQ(snapshotPtr->phase, phase);
                ASSERT_EQ(snapshotPtr->current_round_number, current_round);
                ASSERT_EQ(snapshotPtr->total_rounds, total_rounds);
                ASSERT_EQ(snapshotPtr->time_left, 20);
                for (size_t i = 0; i < ct.size(); i++) {
                    validate_player(ct[i], snapshotPtr->ct[i]);
                }
                for (size_t i = 0; i < ct.size(); i++) {
                    validate_player(tt[i], snapshotPtr->tt[i]);
                }
            }
        }
    }
}
