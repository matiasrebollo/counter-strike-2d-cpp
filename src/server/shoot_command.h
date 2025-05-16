#ifndef SHOOT_COMMAND_H
#define SHOOT_COMMAND_H

#include <string>

#include "server/command.h"

struct ShootCommand: public Command {
    explicit ShootCommand(std::string& username);

    void execute(CS2DGame& game) const override;

    ShootCommand(const ShootCommand&) = delete;
    ShootCommand& operator=(const ShootCommand&) = delete;

    ~ShootCommand();
};

#endif
