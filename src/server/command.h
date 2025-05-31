#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include <string>
#include <variant>

#include "common/commands_dto.h"

class CS2DGame;  // Forward declaration

enum GameCommandType { AttackPhase, BuyPhase };

class Command {
public:
    const std::string username;

    explicit Command(const std::string& username);

    static std::unique_ptr<Command> new_command(const std::string& username,
                                                const CommandDTO& command_data);

    virtual GameCommandType type() const = 0;
    virtual void execute(CS2DGame& game) const = 0;

    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    virtual ~Command();
};

#endif
