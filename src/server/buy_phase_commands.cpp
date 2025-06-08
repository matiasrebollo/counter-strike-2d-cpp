#include "server/buy_phase_commands.h"

// #include "server/game_world.h"

BuyPhaseCommand::BuyPhaseCommand(const std::string& username): Command(username) {}
void BuyPhaseCommand::execute_in_attack_phase(GameWorld&) const {}

BuyGunCommand::BuyGunCommand(const std::string& username, const GunType& gun):
        BuyPhaseCommand(username), gun(gun) {}
void BuyGunCommand::execute_in_buy_phase(GameWorld& game) const { game.buy_gun_for(username, gun); }

BuyAmmoCommand::BuyAmmoCommand(const std::string& username, const bool& for_primary):
        BuyPhaseCommand(username), for_primary(for_primary) {}
void BuyAmmoCommand::execute_in_buy_phase(GameWorld& game) const {
    game.buy_ammo_for(username, for_primary);
}
