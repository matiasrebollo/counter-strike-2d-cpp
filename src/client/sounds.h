#ifndef SOUNDS_H
#define SOUNDS_H

#include <cstdint>
#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/block_texture_parser.h"
#include "../common/settings.h"
#include "../common/sdl_helpers.h"
#include "texture_manager.h"

#define BUTTON_CHANNEL 1


class Sounds {
private:
    struct StepSoundState {
        Uint32 last_step_time = 0;
        bool next_step_left = true;
    };

    std::unordered_map<std::string, StepSoundState> step_states;
    std::unordered_map<std::string, int> player_step_channel;
    int next_step_channel = 1;
    Uint32 step_delay = 500;

    SDL2pp::Mixer& mixer;
    TextureManager& texture_manager;
    BlockTextureParser& texture_parser;
    int total_players = 0;

public:
    Sounds(SDL2pp::Mixer& mixer, TextureManager& texture_manager,
           BlockTextureParser& texture_parser);

    void set_total_players(int total_players);

    void play_shop_sound(SoundEffect effect);

    void play_step(const std::string& username, const SDL2pp::Point& destino_camera,
                   bool is_moving);

    
};


#endif
