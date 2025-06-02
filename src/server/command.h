#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include <string>
#include <variant>

#include "common/commands_dto.h"
#include "server/game_world.h"

class Command {
public:
    const std::string username;

    explicit Command(const std::string& username);

    static std::unique_ptr<Command> new_command(const std::string& username,
                                                const CommandDTO& command_data);

    virtual void execute_in_buy_phase(GameWorld& game) const = 0;
    virtual void execute_in_attack_phase(GameWorld& game) const = 0;

    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    virtual ~Command();
};

#endif
