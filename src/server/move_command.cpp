#include "server/move_command.h"

#include <stdexcept>

#include "server/cs2d_game.h"

MoveCommand::MoveCommand(Vector2D& direction, std::string& username):
        Command(username), direction(direction) {
    if (direction.magnitude() != 1)
        throw std::invalid_argument("Move command direction should be a unit vector");
}

void MoveCommand::execute(CS2DGame& game) const { game.move_player(username, direction); }

MoveCommand::~MoveCommand() {}
