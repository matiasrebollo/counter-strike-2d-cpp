#ifndef ATTACK_PHASE_COMMANDS_H
#define ATTACK_PHASE_COMMANDS_H

#include <string>

#include "server/command.h"

class AttackPhaseCommand: public Command {
public:
    explicit AttackPhaseCommand(const std::string& username);
    GameCommandType type() const override;
    ~AttackPhaseCommand() override = default;
};

class MoveUpCommand: public AttackPhaseCommand {
public:
    explicit MoveUpCommand(const std::string& username);
    void execute(CS2DGame& game) const override;
    ~MoveUpCommand() override = default;
};

class MoveDownCommand: public AttackPhaseCommand {
public:
    explicit MoveDownCommand(const std::string& username);
    void execute(CS2DGame& game) const override;
    ~MoveDownCommand() override = default;
};

class MoveLeftCommand: public AttackPhaseCommand {
public:
    explicit MoveLeftCommand(const std::string& username);
    void execute(CS2DGame& game) const override;
    ~MoveLeftCommand() override = default;
};

class MoveRightCommand: public AttackPhaseCommand {
public:
    explicit MoveRightCommand(const std::string& username);
    void execute(CS2DGame& game) const override;
    ~MoveRightCommand() override = default;
};

class RotateCommand: public AttackPhaseCommand {
public:
    const double angle;
    RotateCommand(const std::string& username, const double& angle);
    void execute(CS2DGame& game) const override;
    ~RotateCommand() override = default;
};

/*struct ShootCommand: public Command {
    explicit ShootCommand(std::string& username);

    void execute(CS2DGame& game) const override;

    ShootCommand(const ShootCommand&) = delete;
    ShootCommand& operator=(const ShootCommand&) = delete;

    ~ShootCommand() = default;
};*/

#endif
