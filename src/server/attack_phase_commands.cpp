#include "server/attack_phase_commands.h"

#include "server/cs2d_game.h"


AttackPhaseCommand::AttackPhaseCommand(const std::string& username): Command(username) {}
GameCommandType AttackPhaseCommand::type() const { return AttackPhase; }

MoveUpCommand::MoveUpCommand(const std::string& username): AttackPhaseCommand(username) {}

void MoveUpCommand::execute(GameWorld& game) const { game.move_player_up(username); }


MoveDownCommand::MoveDownCommand(const std::string& username): AttackPhaseCommand(username) {}

void MoveDownCommand::execute(GameWorld& game) const { game.move_player_down(username); }


MoveLeftCommand::MoveLeftCommand(const std::string& username): AttackPhaseCommand(username) {}

void MoveLeftCommand::execute(GameWorld& game) const { game.move_player_left(username); }


MoveRightCommand::MoveRightCommand(const std::string& username): AttackPhaseCommand(username) {}

void MoveRightCommand::execute(GameWorld& game) const { game.move_player_right(username); }


RotateCommand::RotateCommand(const std::string& username, const double& angle):
        AttackPhaseCommand(username), angle(angle) {}

void RotateCommand::execute(GameWorld& game) const { game.rotate_player(username, angle); }

/*ShootCommand::ShootCommand(std::string& username): Command(username) {}

void ShootCommand::execute(GameWorld& game) const { game.shoot(username); }*/
