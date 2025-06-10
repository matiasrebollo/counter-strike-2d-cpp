#include "server/command.h"

#include "server/attack_phase_commands.h"
#include "server/buy_phase_commands.h"

template <typename>
inline constexpr bool always_false_v = false;

Command::Command(const std::string& username): username(username) {}

std::unique_ptr<Command> Command::new_command(const std::string& username,
                                              const CommandDTO& command_data) {
    return std::visit(
            [username](const auto& d) -> std::unique_ptr<Command> {
                using T = std::decay_t<decltype(d)>;

                if constexpr (std::is_same_v<T, MoveDTO>) {
                    return std::make_unique<MoveCommand>(username, d.dir, d.move);
                } else if constexpr (std::is_same_v<T, RotateDTO>) {
                    return std::make_unique<RotateCommand>(username, d.angle);
                } else if constexpr (std::is_same_v<T, PlayerActionDTO>) {
                    return std::make_unique<PlayerActionCommand>(username, d.make);
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
                } else {
                    static_assert(always_false_v<T>, "Unhandled command data type");
                }
            },
            command_data);
}

Command::~Command() {}
