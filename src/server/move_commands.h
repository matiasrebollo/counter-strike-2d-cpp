#ifndef MOVE_COMMANDS_H
#define MOVE_COMMANDS_H

#include <string>

#include "server/command.h"

class MoveUpCommand: public Command {
public:
    explicit MoveUpCommand(const std::string& username);
    void execute(CS2DGame& game) const override;
    ~MoveUpCommand() override = default;
};

class MoveDownCommand: public Command {
public:
    explicit MoveDownCommand(const std::string& username);
    void execute(CS2DGame& game) const override;
    ~MoveDownCommand() override = default;
};

class MoveLeftCommand: public Command {
public:
    explicit MoveLeftCommand(const std::string& username);
    void execute(CS2DGame& game) const override;
    ~MoveLeftCommand() override = default;
};

class MoveRightCommand: public Command {
public:
    explicit MoveRightCommand(const std::string& username);
    void execute(CS2DGame& game) const override;
    ~MoveRightCommand() override = default;
};

class RotateCommand: public Command {
public:
    const double angle;
    RotateCommand(const std::string& username, const double& angle);
    void execute(CS2DGame& game) const override;
    ~RotateCommand() override = default;
};

#endif
