#include "server/rotate_command.h"

#include <stdexcept>

#include "server/cs2d_game.h"

RotateCommand::RotateCommand(Vector2D& direction, std::string& username):
        Command(username), direction(direction) {
    if (direction.magnitude() != 1)
        throw std::invalid_argument("Rotate command direction should be a unit vector");
}

void RotateCommand::execute(CS2DGame& game) const { game.rotate_player(username, direction); }

RotateCommand::~RotateCommand() {}
