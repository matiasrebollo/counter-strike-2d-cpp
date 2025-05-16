#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include <string>

#include "server/command.h"
#include "server/vector_2d.h"

struct MoveCommand: public Command {
    const Vector2D direction;

    MoveCommand(Vector2D& direction, std::string& username);

    void execute(CS2DGame& game) const override;

    MoveCommand(const MoveCommand&) = delete;
    MoveCommand& operator=(const MoveCommand&) = delete;

    ~MoveCommand();
};

#endif
