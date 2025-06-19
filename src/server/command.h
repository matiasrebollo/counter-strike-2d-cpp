#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include <string>
#include <variant>

#include "common/game_commands_dto.h"
#include "server/game_world.h"

class Command {
protected:
    explicit Command(const std::string& username);

public:
    const std::string username;
    static std::unique_ptr<Command> new_command(const std::string& username,
                                                const GameCommandDTO& command_data);

    virtual void execute_in_buy_phase(GameWorld& game) const;
    virtual void execute_in_attack_phase(GameWorld& game) const;

    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    virtual ~Command();
};

class MoveCommand: public Command {
public:
    const Movement direction;
    const bool should_move;

    MoveCommand(const std::string& username, const Movement direction, const bool& move);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~MoveCommand() override = default;
};

class RotateCommand: public Command {
public:
    const double angle;
    RotateCommand(const std::string& username, const double& angle);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~RotateCommand() override = default;
};

class PlayerActionCommand: public Command {
public:
    const bool make;
    PlayerActionCommand(const std::string& username, const bool& make);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~PlayerActionCommand() override = default;
};

class DefuseBombCommand: public Command {
public:
    const bool make;
    DefuseBombCommand(const std::string& username, const bool& make);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~DefuseBombCommand() override = default;
};

class BuyGunCommand: public Command {
public:
    const GunType gun;
    BuyGunCommand(const std::string& username, const GunType& gun);
    void execute_in_buy_phase(GameWorld& game) const override;
    ~BuyGunCommand() override = default;
};

class BuyAmmoCommand: public Command {
public:
    const bool for_primary;
    BuyAmmoCommand(const std::string& username, const bool& for_primary);
    void execute_in_buy_phase(GameWorld& game) const override;
    ~BuyAmmoCommand() override = default;
};

class EquipCommand: public Command {
protected:
    virtual void execute(GameWorld& game) const = 0;

public:
    explicit EquipCommand(const std::string& username);
    void execute_in_attack_phase(GameWorld& game) const override;
    void execute_in_buy_phase(GameWorld& game) const override;
    ~EquipCommand() override = default;
};
class EquipPrimaryCommand: public EquipCommand {
private:
    void execute(GameWorld& game) const override;

public:
    explicit EquipPrimaryCommand(const std::string& username);
    ~EquipPrimaryCommand() override = default;
};

class EquipSecondaryCommand: public EquipCommand {
private:
    void execute(GameWorld& game) const override;

public:
    explicit EquipSecondaryCommand(const std::string& username);
    ~EquipSecondaryCommand() override = default;
};

class EquipKnifeCommand: public EquipCommand {
private:
    void execute(GameWorld& game) const override;

public:
    explicit EquipKnifeCommand(const std::string& username);
    ~EquipKnifeCommand() override = default;
};

class EquipBombCommand: public EquipCommand {
private:
    void execute(GameWorld& game) const override;

public:
    explicit EquipBombCommand(const std::string& username);
    ~EquipBombCommand() override = default;
};

class PickUpItemCommand: public Command {
public:
    explicit PickUpItemCommand(const std::string& username);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~PickUpItemCommand() override = default;
};


#endif
