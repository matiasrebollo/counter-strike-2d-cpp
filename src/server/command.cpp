#include "server/command.h"

template <typename>
inline constexpr bool always_false_v = false;

Command::Command(const std::string& username): username(username) {}

std::unique_ptr<Command> Command::new_command(const std::string& username,
                                              const GameCommandDTO& command_data) {
    return std::visit(
            [username](const auto& d) -> std::unique_ptr<Command> {
                using T = std::decay_t<decltype(d)>;

                if constexpr (std::is_same_v<T, MoveDTO>) {
                    return std::make_unique<MoveCommand>(username, d.dir, d.move);
                } else if constexpr (std::is_same_v<T, RotateDTO>) {
                    return std::make_unique<RotateCommand>(username, d.angle);
                } else if constexpr (std::is_same_v<T, PlayerActionDTO>) {
                    return std::make_unique<PlayerActionCommand>(username, d.make);
                } else if constexpr (std::is_same_v<T, DefuseBombDTO>) {
                    return std::make_unique<DefuseBombCommand>(username, d.make);
                } else if constexpr (std::is_same_v<T, EquipPrimaryDTO>) {
                    return std::make_unique<EquipPrimaryCommand>(username);
                } else if constexpr (std::is_same_v<T, EquipSecondaryDTO>) {
                    return std::make_unique<EquipSecondaryCommand>(username);
                } else if constexpr (std::is_same_v<T, EquipKnifeDTO>) {
                    return std::make_unique<EquipKnifeCommand>(username);
                } else if constexpr (std::is_same_v<T, EquipBombDTO>) {
                    return std::make_unique<EquipBombCommand>(username);
                } else if constexpr (std::is_same_v<T, BuyGunDTO>) {
                    return std::make_unique<BuyGunCommand>(username, d.gun);
                } else if constexpr (std::is_same_v<T, BuyAmmoDTO>) {
                    return std::make_unique<BuyAmmoCommand>(username, d.for_primary);
                } else if constexpr (std::is_same_v<T, ForceStartDTO>) {
                    return std::make_unique<ForceStartCommand>(username);
                } else {
                    static_assert(always_false_v<T>, "Unhandled command data type");
                }
            },
            command_data);
}

void Command::execute_in_buy_phase(GameWorld&) const {}
void Command::execute_in_attack_phase(GameWorld&) const {}

Command::~Command() {}

ForceStartCommand::ForceStartCommand(const std::string& username): Command(username) {}

MoveCommand::MoveCommand(const std::string& username, const Movement direction, const bool& move):
        Command(username), direction(direction), should_move(move) {}
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
        Command(username), angle(angle) {}
void RotateCommand::execute_in_attack_phase(GameWorld& game) const {
    game.rotate_player(username, angle);
}


PlayerActionCommand::PlayerActionCommand(const std::string& username, const bool& make):
        Command(username), make(make) {}
void PlayerActionCommand::execute_in_attack_phase(GameWorld& game) const {
    if (make) {
        game.make_player_action(username);
    } else {
        game.stop_making_player_action(username);
    }
}

DefuseBombCommand::DefuseBombCommand(const std::string& username, const bool& make):
        Command(username), make(make) {}
void DefuseBombCommand::execute_in_attack_phase(GameWorld& game) const {
    if (make) {
        game.make_player_defuse_bomb(username);
    } else {
        game.stop_making_player_defuse_bomb(username);
    }
}

BuyGunCommand::BuyGunCommand(const std::string& username, const GunType& gun):
        Command(username), gun(gun) {}
void BuyGunCommand::execute_in_buy_phase(GameWorld& game) const { game.buy_gun_for(username, gun); }

BuyAmmoCommand::BuyAmmoCommand(const std::string& username, const bool& for_primary):
        Command(username), for_primary(for_primary) {}
void BuyAmmoCommand::execute_in_buy_phase(GameWorld& game) const {
    game.buy_ammo_for(username, for_primary);
}

EquipCommand::EquipCommand(const std::string& username): Command(username) {}
void EquipCommand::execute_in_attack_phase(GameWorld& game) const { execute(game); }
void EquipCommand::execute_in_buy_phase(GameWorld& game) const { execute(game); }

EquipPrimaryCommand::EquipPrimaryCommand(const std::string& username): EquipCommand(username) {}
void EquipPrimaryCommand::execute(GameWorld& game) const { game.equip_primary_for(username); }

EquipSecondaryCommand::EquipSecondaryCommand(const std::string& username): EquipCommand(username) {}
void EquipSecondaryCommand::execute(GameWorld& game) const { game.equip_secondary_for(username); }

EquipKnifeCommand::EquipKnifeCommand(const std::string& username): EquipCommand(username) {}
void EquipKnifeCommand::execute(GameWorld& game) const { game.equip_knife_for(username); }

EquipBombCommand::EquipBombCommand(const std::string& username): EquipCommand(username) {}
void EquipBombCommand::execute(GameWorld& game) const { game.equip_bomb_for(username); }
