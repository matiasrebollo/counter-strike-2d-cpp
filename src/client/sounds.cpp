#include "sounds.h"

#include <algorithm>

Sounds::Sounds(SDL2pp::Mixer& mixer, TextureManager& texture_manager,
               BlockTextureParser& texture_parser):
        mixer(mixer), texture_manager(texture_manager), texture_parser(texture_parser) {}

void Sounds::initialize_channels(const std::vector<std::string>& usernames) {
    int current_channel = 1;

    for (const auto& username: usernames) player_step_channel[username] = current_channel++;

    for (const auto& username: usernames) player_shot_channel[username] = current_channel++;

    shop_channel = current_channel++;
    bomb_channel = current_channel++;
    round_channel = current_channel++;
    clock_channel = current_channel++;
    bomb_action_channel = current_channel++;
    bomb_tick_channel = current_channel;
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
        case SoundType::BOMB_TYPE: {
            return bomb_channel;
        }
        case SoundType::ROUND_TYPE: {
            return round_channel;
        }
        case SoundType::CLOCK_TYPE: {
            return clock_channel;
        }
        case SoundType::BOMB_ACTION_TYPE: {
            return bomb_action_channel;
        }
        case SoundType::BOMB_TICK_TYPE: {
            return bomb_tick_channel;
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

void Sounds::play_round_sound(SoundEffect effect) {
    std::string path = texture_parser.get_sound_path(effect);
    SDL2pp::Chunk& sound = texture_manager.get_sound(path);
    int channel = get_channel(" ", SoundType::ROUND_TYPE);
    mixer.PlayChannel(channel, sound);
}

void Sounds::play_bomb_sound(SoundEffect effect) {
    std::string path = texture_parser.get_sound_path(effect);
    SDL2pp::Chunk& sound = texture_manager.get_sound(path);
    int channel = get_channel(" ", SoundType::BOMB_TYPE);
    mixer.PlayChannel(channel, sound);
}


void Sounds::play_clock_sound(SoundEffect effect) {
    if (clock_playing)
        return;

    std::string path = texture_parser.get_sound_path(effect);
    SDL2pp::Chunk& sound = texture_manager.get_sound(path);
    int channel = get_channel(" ", SoundType::CLOCK_TYPE);
    mixer.PlayChannel(channel, sound);

    clock_playing = true;
}

void Sounds::stop_clock_sound() {
    if (!clock_playing)
        return;

    int channel = get_channel(" ", SoundType::CLOCK_TYPE);
    mixer.HaltChannel(channel);

    clock_playing = false;
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

    std::string path;
    int ticks = 100000;
    if (gun_type == GLOCK) {
        path = texture_parser.get_sound_path(GLOCK_SHOT);
    } else if (gun_type == AWP) {
        path = texture_parser.get_sound_path(AWP_SHOT);
    } else if (gun_type == NONE) {
        path = texture_parser.get_sound_path(KNIFE_HIT);
    } else if (gun_type == M3) {
        path = texture_parser.get_sound_path(M3_SHOT);
        ticks = 1000;
    } else if (gun_type == AK47) {
        path = texture_parser.get_sound_path(AK_SHOT);
    }

    SDL2pp::Chunk& sound = texture_manager.get_sound(path);

    int used_channel = mixer.PlayChannel(channel, sound, 0, ticks);
    if (used_channel != -1) {
        mixer.SetDistance(used_channel, sdl_distance);
    }
}

void Sounds::play_bomb_explosion(const SDL2pp::Point& destino_camera) {

    int channel = get_channel(" ", SoundType::BOMB_ACTION_TYPE);
    if (channel == -1)
        return;

    int cam_center_x = CAMERA_WIDTH / 2;
    int cam_center_y = CAMERA_HEIGHT / 2;

    int dx = destino_camera.x - cam_center_x;
    int dy = destino_camera.y - cam_center_y;
    double dist = std::sqrt(dx * dx + dy * dy);

    double max_hearing_distance = 6 * std::sqrt((CAMERA_WIDTH / 2.0) * (CAMERA_WIDTH / 2.0) +
                                                (CAMERA_HEIGHT / 2.0) * (CAMERA_HEIGHT / 2.0));
    Uint8 sdl_distance = static_cast<Uint8>(std::min(255.0, (dist / max_hearing_distance) * 255.0));

    std::string path = texture_parser.get_sound_path(BOMB_EXPLOSION);
    SDL2pp::Chunk& sound = texture_manager.get_sound(path);

    int used_channel = mixer.PlayChannel(channel, sound);
    if (used_channel != -1) {
        mixer.SetDistance(used_channel, sdl_distance);
    }
}

void Sounds::play_bomb_action(const SDL2pp::Point& destino_camera, bool is_planting,
                              bool is_defusing) {
    if (!is_planting && !is_defusing)
        return;

    Uint32 now = SDL_GetTicks();
    if (now - last_bomb_action_time < bomb_action_delay)
        return;

    int channel = get_channel("bomb_action", SoundType::BOMB_ACTION_TYPE);
    if (channel == -1)
        return;

    int cam_center_x = CAMERA_WIDTH / 2;
    int cam_center_y = CAMERA_HEIGHT / 2;

    int dx = destino_camera.x - cam_center_x;
    int dy = destino_camera.y - cam_center_y;
    double dist = std::sqrt(dx * dx + dy * dy);

    double max_hearing_distance = 0.5 * std::sqrt((CAMERA_WIDTH / 2.0) * (CAMERA_WIDTH / 2.0) +
                                                  (CAMERA_HEIGHT / 2.0) * (CAMERA_HEIGHT / 2.0));
    Uint8 sdl_distance = static_cast<Uint8>(std::min(255.0, (dist / max_hearing_distance) * 255.0));

    std::string path = texture_parser.get_sound_path(BOMB_ACTION);
    SDL2pp::Chunk& sound = texture_manager.get_sound(path);
    sound.SetVolume(MIX_MAX_VOLUME / 2);

    int used_channel = mixer.PlayChannel(channel, sound);
    if (used_channel != -1) {
        mixer.SetDistance(used_channel, sdl_distance);
    }

    last_bomb_action_time = now;
}

void Sounds::play_bomb_tick(const SDL2pp::Point& destino_camera) {
    Uint32 now = SDL_GetTicks();
    if (now - last_bomb_tick_time < bomb_tick_delay)
        return;

    int channel = get_channel("bomb_tick", SoundType::BOMB_TICK_TYPE);
    if (channel == -1)
        return;

    int cam_center_x = CAMERA_WIDTH / 2;
    int cam_center_y = CAMERA_HEIGHT / 2;

    int dx = destino_camera.x - cam_center_x;
    int dy = destino_camera.y - cam_center_y;
    double dist = std::sqrt(dx * dx + dy * dy);

    double max_hearing_distance = 0.5 * std::sqrt((CAMERA_WIDTH / 2.0) * (CAMERA_WIDTH / 2.0) +
                                                  (CAMERA_HEIGHT / 2.0) * (CAMERA_HEIGHT / 2.0));
    Uint8 sdl_distance = static_cast<Uint8>(std::min(255.0, (dist / max_hearing_distance) * 255.0));

    std::string path = texture_parser.get_sound_path(BOMB_TICK);
    SDL2pp::Chunk& sound = texture_manager.get_sound(path);
    sound.SetVolume(MIX_MAX_VOLUME / 2);

    int used_channel = mixer.PlayChannel(channel, sound);
    if (used_channel != -1) {
        mixer.SetDistance(used_channel, sdl_distance);
    }

    last_bomb_tick_time = now;
}

void Sounds::play_death(const std::string& username, const SDL2pp::Point& destino_camera) {
    int channel = get_channel(username, SoundType::STEP_TYPE);
    if (channel == -1)
        return;

    int cam_center_x = CAMERA_WIDTH / 2;
    int cam_center_y = CAMERA_HEIGHT / 2;
    int dx = destino_camera.x - cam_center_x;
    int dy = destino_camera.y - cam_center_y;
    double dist = std::sqrt(dx * dx + dy * dy);

    double max_hearing_distance = 0.4 * std::sqrt((CAMERA_WIDTH / 2.0) * (CAMERA_WIDTH / 2.0) +
                                                  (CAMERA_HEIGHT / 2.0) * (CAMERA_HEIGHT / 2.0));
    Uint8 sdl_distance = static_cast<Uint8>(std::min(255.0, (dist / max_hearing_distance) * 255.0));

    std::array<SoundEffect, 3> death_sounds = {DEATH_SOUND_ONE, DEATH_SOUND_TWO, DEATH_SOUND_THREE};
    int index = std::rand() % death_sounds.size();
    SoundEffect selected = death_sounds[index];

    std::string path = texture_parser.get_sound_path(selected);
    SDL2pp::Chunk& sound = texture_manager.get_sound(path);

    int used_channel = mixer.PlayChannel(channel, sound);
    if (used_channel != -1) {
        mixer.SetDistance(used_channel, sdl_distance);
    }
}
