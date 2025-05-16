#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class CS2DGame;  // Forward declaration

class Command {

public:
    const std::string username;

    explicit Command(std::string& username): username(username) {}

    virtual void execute(CS2DGame& game) const = 0;

    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    virtual ~Command() {}
};

#endif
