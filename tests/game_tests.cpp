#include <gtest/gtest.h>

#include "common_tests.h"

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
