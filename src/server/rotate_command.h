#ifndef ROTATE_COMMAND_H
#define ROTATE_COMMAND_H

#include <string>

#include "common/vector_2d.h"
#include "server/command.h"

struct RotateCommand: public Command {
    const Vector2D direction;

    RotateCommand(Vector2D& direction, std::string& username);

    void execute(CS2DGame& game) const override;

    RotateCommand(const RotateCommand&) = delete;
    RotateCommand& operator=(const RotateCommand&) = delete;

    ~RotateCommand();
};

#endif
