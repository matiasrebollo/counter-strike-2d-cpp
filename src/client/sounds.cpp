#include "sounds.h"

#include <algorithm>

Sounds::Sounds(SDL2pp::Mixer& mixer, TextureManager& texture_manager,
               BlockTextureParser& texture_parser):
        mixer(mixer), texture_manager(texture_manager), texture_parser(texture_parser) {}

void Sounds::initialize_channels(const std::vector<std::string>& usernames) {
    int current_channel = 1;

    for (const auto& username: usernames) player_step_channel[username] = current_channel++;

    for (const auto& username: usernames) player_shot_channel[username] = current_channel++;

    shop_channel = current_channel;

    total_players = usernames.size();
}

int Sounds::get_channel(const std::string& username, SoundType type) const {
    switch (type) {
        case SoundType::STEP_TYPE: {
            auto it = player_step_channel.find(username);
            if (it != player_step_channel.end())
                return it->second;
            break;
        }
        case SoundType::SHOT_TYPE: {
            auto it = player_shot_channel.find(username);
            if (it != player_shot_channel.end())
                return it->second;
            break;
        }
        case SoundType::SHOP_TYPE: {
            return shop_channel;
        }
    }
    return -1;
}

void Sounds::play_shop_sound(SoundEffect effect) {
    std::string path = texture_parser.get_sound_path(effect);
    SDL2pp::Chunk& sound = texture_manager.get_sound(path);
    int channel = get_channel(" ", SoundType::SHOP_TYPE);
    mixer.PlayChannel(channel, sound);
}


void Sounds::play_step(const std::string& username, const SDL2pp::Point& destino_camera,
                       bool is_moving) {
    if (!is_moving)
        return;

    Uint32 now = SDL_GetTicks();
    auto& state = step_states[username];

    if (now - state.last_step_time < step_delay)
        return;

    int channel = get_channel(username, SoundType::STEP_TYPE);
    if (channel == -1)
        return;

    int cam_center_x = CAMERA_WIDTH / 2;
    int cam_center_y = CAMERA_HEIGHT / 2;

    int dx = destino_camera.x - cam_center_x;
    int dy = destino_camera.y - cam_center_y;
    double dist = std::sqrt(dx * dx + dy * dy);

    double max_hearing_distance = std::sqrt((CAMERA_WIDTH / 2.0) * (CAMERA_WIDTH / 2.0) +
                                            (CAMERA_HEIGHT / 2.0) * (CAMERA_HEIGHT / 2.0));
    Uint8 sdl_distance = static_cast<Uint8>(std::min(255.0, (dist / max_hearing_distance) * 255.0));

    std::string path =
            texture_parser.get_sound_path(state.next_step_left ? DIRT_STEP_ONE : DIRT_STEP_TWO);
    SDL2pp::Chunk& sound = texture_manager.get_sound(path);

    int used_channel = mixer.PlayChannel(channel, sound);
    if (used_channel != -1) {
        mixer.SetDistance(used_channel, sdl_distance);
    }

    state.last_step_time = now;
    state.next_step_left = !state.next_step_left;
}

void Sounds::play_shot(const std::string& username, GunType gun_type,
                       const SDL2pp::Point& destino_camera) {
    if (gun_type != GLOCK)  // por ahora solo Glock
        return;

    int channel = get_channel(username, SoundType::SHOT_TYPE);
    if (channel == -1)
        return;

    int cam_center_x = CAMERA_WIDTH / 2;
    int cam_center_y = CAMERA_HEIGHT / 2;

    int dx = destino_camera.x - cam_center_x;
    int dy = destino_camera.y - cam_center_y;
    double dist = std::sqrt(dx * dx + dy * dy);

    double max_hearing_distance = 3 * std::sqrt((CAMERA_WIDTH / 2.0) * (CAMERA_WIDTH / 2.0) +
                                                (CAMERA_HEIGHT / 2.0) * (CAMERA_HEIGHT / 2.0));
    Uint8 sdl_distance = static_cast<Uint8>(std::min(255.0, (dist / max_hearing_distance) * 255.0));

    std::string path = texture_parser.get_sound_path(GLOCK_SHOT);
    SDL2pp::Chunk& sound = texture_manager.get_sound(path);

    int used_channel = mixer.PlayChannel(channel, sound);
    if (used_channel != -1) {
        mixer.SetDistance(used_channel, sdl_distance);
    }
}
