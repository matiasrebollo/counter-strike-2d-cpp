#ifndef BUY_PHASE_COMMANDS_H
#define BUY_PHASE_COMMANDS_H

#include <string>

#include "server/command.h"

class BuyPhaseCommand: public Command {
public:
    explicit BuyPhaseCommand(const std::string& username);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~BuyPhaseCommand() override = default;
};

class BuyGunCommand: public BuyPhaseCommand {
public:
    const GunType gun;
    BuyGunCommand(const std::string& username, const GunType& gun);
    void execute_in_buy_phase(GameWorld& game) const override;
    ~BuyGunCommand() override = default;
};

class BuyAmmoCommand: public BuyPhaseCommand {
public:
    const bool for_primary;
    BuyAmmoCommand(const std::string& username, const bool& for_primary);
    void execute_in_buy_phase(GameWorld& game) const override;
    ~BuyAmmoCommand() override = default;
};

#endif
