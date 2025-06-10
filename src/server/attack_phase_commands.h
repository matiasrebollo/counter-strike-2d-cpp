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

class MoveCommand: public AttackPhaseCommand {
public:
    const Movement direction;
    const bool should_move;

    MoveCommand(const std::string& username, const Movement direction, const bool& move);
    void execute_in_attack_phase(GameWorld& game) const override;
    ~MoveCommand() override = default;
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
    const bool make;
    PlayerActionCommand(const std::string& username, const bool& make);
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
