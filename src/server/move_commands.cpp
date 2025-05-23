#include "server/move_commands.h"

#include "server/cs2d_game.h"


MoveUpCommand::MoveUpCommand(const std::string& username): Command(username) {}

void MoveUpCommand::execute(CS2DGame& game) const { game.move_player_up(username); }


MoveDownCommand::MoveDownCommand(const std::string& username): Command(username) {}

void MoveDownCommand::execute(CS2DGame& game) const { game.move_player_down(username); }


MoveLeftCommand::MoveLeftCommand(const std::string& username): Command(username) {}

void MoveLeftCommand::execute(CS2DGame& game) const { game.move_player_left(username); }


MoveRightCommand::MoveRightCommand(const std::string& username): Command(username) {}

void MoveRightCommand::execute(CS2DGame& game) const { game.move_player_right(username); }


RotateCommand::RotateCommand(const std::string& username, const float& angle):
        Command(username), angle(angle) {}

void RotateCommand::execute(CS2DGame& game) const { game.rotate_player(username, angle); }
