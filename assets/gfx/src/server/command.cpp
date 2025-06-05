#include "server/command.h"

#include "server/attack_phase_commands.h"

template <typename>
inline constexpr bool always_false_v = false;

Command::Command(const std::string& username): username(username) {}

std::unique_ptr<Command> Command::new_command(const std::string& username,
                                              const CommandDTO& command_data) {
    return std::visit(
            [username](const auto& d) -> std::unique_ptr<Command> {
                using T = std::decay_t<decltype(d)>;

                if constexpr (std::is_same_v<T, MoveUpDTO>) {
                    return std::make_unique<MoveUpCommand>(username);
                } else if constexpr (std::is_same_v<T, MoveDownDTO>) {
                    return std::make_unique<MoveDownCommand>(username);
                } else if constexpr (std::is_same_v<T, MoveLeftDTO>) {
                    return std::make_unique<MoveLeftCommand>(username);
                } else if constexpr (std::is_same_v<T, MoveRightDTO>) {
                    return std::make_unique<MoveRightCommand>(username);
                } else if constexpr (std::is_same_v<T, RotateDTO>) {
                    return std::make_unique<RotateCommand>(username, d.angle);
                } else {
                    static_assert(always_false_v<T>, "Unhandled command data type");
                }
            },
            command_data);
}

Command::~Command() {}
