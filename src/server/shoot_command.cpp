#include "server/shoot_command.h"

#include "server/cs2d_game.h"

ShootCommand::ShootCommand(std::string& username): Command(username) {}

void ShootCommand::execute(CS2DGame& game) const { game.shoot(username); }

ShootCommand::~ShootCommand() {}
