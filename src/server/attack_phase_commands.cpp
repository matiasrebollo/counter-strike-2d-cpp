#include "server/attack_phase_commands.h"

// #include "server/game_world.h"


AttackPhaseCommand::AttackPhaseCommand(const std::string& username): Command(username) {}
void AttackPhaseCommand::execute_in_buy_phase(GameWorld&) const {}

MoveUpCommand::MoveUpCommand(const std::string& username): AttackPhaseCommand(username) {}
void MoveUpCommand::execute_in_attack_phase(GameWorld& game) const {
    game.move_player_up(username);
}

MoveDownCommand::MoveDownCommand(const std::string& username): AttackPhaseCommand(username) {}
void MoveDownCommand::execute_in_attack_phase(GameWorld& game) const {
    game.move_player_down(username);
}

MoveLeftCommand::MoveLeftCommand(const std::string& username): AttackPhaseCommand(username) {}
void MoveLeftCommand::execute_in_attack_phase(GameWorld& game) const {
    game.move_player_left(username);
}

MoveRightCommand::MoveRightCommand(const std::string& username): AttackPhaseCommand(username) {}
void MoveRightCommand::execute_in_attack_phase(GameWorld& game) const {
    game.move_player_right(username);
}

RotateCommand::RotateCommand(const std::string& username, const double& angle):
        AttackPhaseCommand(username), angle(angle) {}
void RotateCommand::execute_in_attack_phase(GameWorld& game) const {
    game.rotate_player(username, angle);
}


PlayerActionCommand::PlayerActionCommand(const std::string& username):
        AttackPhaseCommand(username) {}
void PlayerActionCommand::execute_in_attack_phase(GameWorld& /*game*/) const {}

EquipPrimaryCommand::EquipPrimaryCommand(const std::string& username):
        AttackPhaseCommand(username) {}
void EquipPrimaryCommand::execute_in_attack_phase(GameWorld& /*game*/) const {}

EquipSecondaryCommand::EquipSecondaryCommand(const std::string& username):
        AttackPhaseCommand(username) {}
void EquipSecondaryCommand::execute_in_attack_phase(GameWorld& /*game*/) const {}

EquipKnifeCommand::EquipKnifeCommand(const std::string& username): AttackPhaseCommand(username) {}
void EquipKnifeCommand::execute_in_attack_phase(GameWorld& /*game*/) const {}

EquipBombCommand::EquipBombCommand(const std::string& username): AttackPhaseCommand(username) {}
void EquipBombCommand::execute_in_attack_phase(GameWorld& /*game*/) const {}
