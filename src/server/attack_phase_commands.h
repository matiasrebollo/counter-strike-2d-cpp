#ifndef ATTACK_PHASE_COMMANDS_H
#define ATTACK_PHASE_COMMANDS_H

#include <string>

#include "server/command.h"

class AttackPhaseCommand: public Command {
public:
    explicit AttackPhaseCommand(const std::string& username);
    void execute_in_buy_phase(GameWorld& game) const override;
    ~AttackPhaseCommand() override = default;
};

class MoveUpCommand: public AttackPhaseCommand {
public:
    explicit MoveUpCommand(const std::string& username);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~MoveUpCommand() override = default;
};

class MoveDownCommand: public AttackPhaseCommand {
public:
    explicit MoveDownCommand(const std::string& username);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~MoveDownCommand() override = default;
};

class MoveLeftCommand: public AttackPhaseCommand {
public:
    explicit MoveLeftCommand(const std::string& username);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~MoveLeftCommand() override = default;
};

class MoveRightCommand: public AttackPhaseCommand {
public:
    explicit MoveRightCommand(const std::string& username);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~MoveRightCommand() override = default;
};

class RotateCommand: public AttackPhaseCommand {
public:
    const double angle;
    RotateCommand(const std::string& username, const double& angle);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~RotateCommand() override = default;
};

class PlayerActionCommand: public AttackPhaseCommand {
public:
    explicit PlayerActionCommand(const std::string& username);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~PlayerActionCommand() override = default;
};

class EquipPrimaryCommand: public AttackPhaseCommand {
public:
    explicit EquipPrimaryCommand(const std::string& username);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~EquipPrimaryCommand() override = default;
};

class EquipSecondaryCommand: public AttackPhaseCommand {
public:
    explicit EquipSecondaryCommand(const std::string& username);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~EquipSecondaryCommand() override = default;
};

class EquipKnifeCommand: public AttackPhaseCommand {
public:
    explicit EquipKnifeCommand(const std::string& username);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~EquipKnifeCommand() override = default;
};

class EquipBombCommand: public AttackPhaseCommand {
public:
    explicit EquipBombCommand(const std::string& username);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~EquipBombCommand() override = default;
};

#endif
