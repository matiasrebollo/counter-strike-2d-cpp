#include "server/attack_phase_commands.h"


AttackPhaseCommand::AttackPhaseCommand(const std::string& username): Command(username) {}
void AttackPhaseCommand::execute_in_buy_phase(GameWorld&) const {}

MoveCommand::MoveCommand(const std::string& username, const Movement direction, const bool& move):
        AttackPhaseCommand(username), direction(direction), should_move(move) {}
void MoveCommand::execute_in_attack_phase(GameWorld& game) const {
    if (should_move) {
        if (direction == UP)
            game.move_player_up(username);
        if (direction == DOWN)
            game.move_player_down(username);
        if (direction == LEFT)
            game.move_player_left(username);
        if (direction == RIGHT)
            game.move_player_right(username);
    } else {
        if (direction == UP)
            game.stop_moving_player_up(username);
        if (direction == DOWN)
            game.stop_moving_player_down(username);
        if (direction == LEFT)
            game.stop_moving_player_left(username);
        if (direction == RIGHT)
            game.stop_moving_player_right(username);
    }
}

RotateCommand::RotateCommand(const std::string& username, const double& angle):
        AttackPhaseCommand(username), angle(angle) {}
void RotateCommand::execute_in_attack_phase(GameWorld& game) const {
    game.rotate_player(username, angle);
}


PlayerActionCommand::PlayerActionCommand(const std::string& username, const bool& make):
        AttackPhaseCommand(username), make(make) {}
void PlayerActionCommand::execute_in_attack_phase(GameWorld& game) const {
    if (make) {
        game.make_player_action(username);
    } else {
        game.stop_making_player_action(username);
    }
}

EquipPrimaryCommand::EquipPrimaryCommand(const std::string& username):
        AttackPhaseCommand(username) {}
void EquipPrimaryCommand::execute_in_attack_phase(GameWorld& game) const {
    game.equip_primary_for(username);
}

EquipSecondaryCommand::EquipSecondaryCommand(const std::string& username):
        AttackPhaseCommand(username) {}
void EquipSecondaryCommand::execute_in_attack_phase(GameWorld& game) const {
    game.equip_secondary_for(username);
}

EquipKnifeCommand::EquipKnifeCommand(const std::string& username): AttackPhaseCommand(username) {}
void EquipKnifeCommand::execute_in_attack_phase(GameWorld& game) const {
    game.equip_knife_for(username);
}

EquipBombCommand::EquipBombCommand(const std::string& username): AttackPhaseCommand(username) {}
void EquipBombCommand::execute_in_attack_phase(GameWorld& /*game*/) const {}
