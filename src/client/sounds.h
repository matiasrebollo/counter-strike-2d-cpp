#ifndef SOUNDS_H
#define SOUNDS_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/block_texture_parser.h"
#include "../common/sdl_helpers.h"
#include "../common/settings.h"

#include "texture_manager.h"

enum SoundType {
    STEP_TYPE,
    SHOT_TYPE,
    SHOP_TYPE,
    ROUND_TYPE,
    BOMB_TYPE,
    CLOCK_TYPE,
    BOMB_ACTION_TYPE,
    BOMB_TICK_TYPE
};


class Sounds {
private:
    struct StepSoundState {
        Uint32 last_step_time = 0;
        bool next_step_left = true;
    };

    std::unordered_map<std::string, StepSoundState> step_states;
    std::unordered_map<std::string, int> player_step_channel;
    std::unordered_map<std::string, int> player_shot_channel;

    int shop_channel = 0;
    int round_channel = 0;
    int bomb_channel = 0;
    int clock_channel = 0;
    int bomb_action_channel = 0;
    int bomb_tick_channel = 0;
    Uint32 step_delay = 500;
    Uint32 last_bomb_action_time = 0;
    Uint32 bomb_action_delay = 600;
    Uint32 last_bomb_tick_time = 0;
    Uint32 bomb_tick_delay = 1000;
    bool clock_playing = false;


    SDL2pp::Mixer& mixer;
    TextureManager& texture_manager;
    BlockTextureParser& texture_parser;

public:
    Sounds(SDL2pp::Mixer& mixer, TextureManager& texture_manager,
           BlockTextureParser& texture_parser);

    void initialize_channels(const std::vector<std::string>& usernames);

    int get_channel(const std::string& username, SoundType type) const;

    void play_shop_sound(SoundEffect effect);

    void play_round_sound(SoundEffect effect);

    void play_bomb_sound(SoundEffect effect);

    void play_clock_sound(SoundEffect effect);

    void stop_clock_sound();

    void play_step(const std::string& username, const SDL2pp::Point& destino_camera,
                   bool is_moving);

    void play_shot(const std::string& username, GunType gun_type,
                   const SDL2pp::Point& destino_camera);

    void play_bomb_explosion(const SDL2pp::Point& destino_camera);
    void play_bomb_action(const SDL2pp::Point& destino_camera, bool is_planting, bool is_defusing);
    void play_bomb_tick(const SDL2pp::Point& destino_camera);
    void play_death(const std::string& username, const SDL2pp::Point& destino_camera);
};


#endif
