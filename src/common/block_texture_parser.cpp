#include "block_texture_parser.h"

#include <algorithm>
#include <string>
#include <vector>

#include "map_object.h"

#define PATH_ASSETS "../assets/"

BlockTextureParser::BlockTextureParser(): block_textures(), backgrounds_paths() {
    this->set_sound_paths();
    this->set_backgrounds_paths();
    this->set_blocks_textures();
    this->set_skins_textures();
    this->set_tileset_numbers();
    this->set_symbols();
    this->set_crosshairs();
    this->set_font_and_waiting();
    this->set_gun_textures();
    this->set_editor_gun_textures();
    this->set_other_paths();
}

const BlockTextureInfo& BlockTextureParser::get_texture_info(int block) {
    return block_textures[block];
}

const BlockTextureInfo& BlockTextureParser::get_ct_texture(CounterTerroristSkin skin,
                                                           Position sprite_index) const {
    return ct_skins.at(skin).at(sprite_index);
}

const BlockTextureInfo& BlockTextureParser::get_tt_texture(TerroristSkin skin,
                                                           Position sprite_index) const {
    return tt_skins.at(skin).at(sprite_index);
}

const BlockTextureInfo& BlockTextureParser::get_number_texture(HudNumbers sprite) const {
    return number_textures.at(sprite);
}

const BlockTextureInfo& BlockTextureParser::get_symbol_texture(HudSymbols symbol) const {
    return symbol_textures.at(symbol);
}

const std::string& BlockTextureParser::get_fw_texture(FontsAndBackground fw) const {
    return font_and_waiting_textures.at(fw);
}

const BlockTextureInfo& BlockTextureParser::get_crosshair_texture(Crosshairs crosshair) const {
    return crosshair_textures.at(crosshair);
}

const std::string& BlockTextureParser::get_gun_texture(GunSprites gun) const {
    return gun_textures.at(gun);
}

const std::vector<int> BlockTextureParser::get_blocks_keys() {
    std::vector<int> keys;
    keys.reserve(block_textures.size());

    std::transform(block_textures.begin(), block_textures.end(), std::back_inserter(keys),
                   [](const auto& pair) { return pair.first; });
    return keys;
}

const std::vector<Background> BlockTextureParser::get_backgrounds() {
    std::vector<Background> keys;
    keys.reserve(backgrounds_paths.size());

    std::transform(backgrounds_paths.begin(), backgrounds_paths.end(), std::back_inserter(keys),
                   [](const auto& pair) { return pair.first; });
    return keys;
}

const std::string& BlockTextureParser::get_background_path(const Background& background) {
    return backgrounds_paths.at(background);
}

const std::string& BlockTextureParser::get_sound_path(SoundEffect effect) const {
    return sound_paths.at(effect);
}

const std::string& BlockTextureParser::get_other_path(Others other) const {
    return other_paths.at(other);
}

void BlockTextureParser::set_other_paths() {
    other_paths = {{AWP_SHOT_FLARE, "../assets/gfx/flare3.bmp"}};
}

void BlockTextureParser::set_sound_paths() {
    sound_paths = {{DENY_SELECT, "../assets/sfx/wpn_denyselect.wav"},
                   {SELECT, "../assets/sfx/wpn_select.wav"},
                   {MOVE_SELECT, "../assets/sfx/wpn_moveselect.wav"},
                   {OPEN_SHOP, "../assets/sfx/wpn_hudon.wav"},
                   {CLOSE_SHOP, "../assets/sfx/wpn_hudoff.wav"},
                   {DIRT_STEP_ONE, "../assets/sfx/player/pl_dirt1.wav"},
                   {DIRT_STEP_TWO, "../assets/sfx/player/pl_dirt2.wav"},
                   {GLOCK_SHOT, "../assets/sfx/weapons/glock18.wav"},
                   {AWP_SHOT, "../assets/sfx/weapons/awp.wav"},
                   {KNIFE_HIT, "../assets/sfx/weapons/knife_hit.wav"},
                   {M3_SHOT, "../assets/sfx/weapons/m3.wav"},
                   {START_ROUND_TT, "../assets/sfx/radio/go.ogg"},
                   {START_ROUND_CT, "../assets/sfx/radio/letsgo.ogg"},
                   {BOMB_DEFUSE, "../assets/sfx/radio/bombdef.ogg"},
                   {BOMB_PLANTED, "../assets/sfx/radio/bombpl.ogg"},
                   {CT_WINS, "../assets/sfx/radio/ctwin.ogg"},
                   {TT_WINS, "../assets/sfx/radio/terwin.ogg"},
                   {FAST_TICK_CLOCK, "../assets/sfx/fast-ticking-clock-sound.wav"}};
}

const std::string& BlockTextureParser::get_editor_gun_texture(const GunType& gun) {
    return editor_gun_textures.at(gun);
}

void BlockTextureParser::set_backgrounds_paths() {
    this->backgrounds_paths = {
            {AZTEC_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/aztec.png"},
            {DUST_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/dust.png"},
            {GRASS_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/gras1.jpg"},
            {BLOOD_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/blood.jpg"},
            {INFERNO_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/inferno.png"},
            {LAVA_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/lava.jpg"},
            {MARS_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/mars.png"},
            {NUKE_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/nuke.png"},
            {OFFICE_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/office.png"},
            {SAND1_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/sand1.jpg"},
            {SAND1_NIGHT_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/sand1-night.jpg"},
            {SNOW_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/snow.jpg"},
            {SPACE_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/space.jpg"},
            {STONE1_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/stone1.jpg"},
            {TOXIC_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/toxic.jpg"},
            {WATER1_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/water1.jpg"},
            {WATER2_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/water2.jpg"},
            {WATER3_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/water3.jpg"},
            {WATER4_BACKGROUND, std::string(PATH_ASSETS) + "gfx/backgrounds/water4.jpg"}};
}

void BlockTextureParser::set_blocks_textures() {
    std::vector<TilesetInfo> tilesets = {
            {std::string(PATH_ASSETS) + "gfx/tiles/aztec.bmp",
             6,
             16,
             32,
             32,
             {1, 2, 3, 4, 5, 6, 7, 8, 47, 46, 45, 93, 92, 88, 87},
             {0, 9, 10, 11, 12, 13, 14, 15, 95, 94, 91, 90, 89, 73}},
            {std::string(PATH_ASSETS) + "gfx/tiles/eldorado.bmp",
             2,
             16,
             32,
             32,
             {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 31, 30, 29},
             {0}},
            {std::string(PATH_ASSETS) + "gfx/tiles/dust_klin.bmp",
             17,
             8,
             32,
             32,
             {1,  2,  3,  4,  5,  6,  7,  23, 22, 21, 20, 63, 62, 61, 60, 59, 58, 66, 67,
              74, 75, 76, 77, 78, 79, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 98, 132},
             {0, 39, 55, 71, 70, 69, 68, 64, 65, 72, 73}},
            {std::string(PATH_ASSETS) + "gfx/tiles/cs_italy_tiles.png",
             15,
             5,
             32,
             32,
             {1, 2, 3, 4, 39, 38, 40, 41, 42, 43, 45, 46, 47, 51, 52},
             {0, 44, 49, 48, 50, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 69, 74}},
            {std::string(PATH_ASSETS) + "gfx/tiles/default_aztec.png",
             10,
             5,
             32,
             32,
             {20, 21, 22, 23, 24, 25, 26, 27, 28, 29},
             {0, 46, 47, 48, 49}},
            {std::string(PATH_ASSETS) + "gfx/tiles/default_dust.png",
             14,
             5,
             32,
             32,
             {20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 60, 61, 62},
             {0, 63, 65, 66, 67, 68}},
            {std::string(PATH_ASSETS) + "gfx/tiles/default_inferno.png",
             15,
             5,
             32,
             32,
             {20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 73, 72, 71, 70},
             {0, 74, 56}}};

    int id_block = 0;
    for (const auto& ts: tilesets) {
        int offset_tileset = id_block;
        for (int row = 0; row < ts.rows; row++) {
            for (int col = 0; col < ts.columns; col++) {
                if (not(ts.invalids.count(id_block - offset_tileset) > 0)) {
                    block_textures[id_block] = {
                            ts.file,
                            col * ts.tileWidth,
                            row * ts.tileHeight,
                            ts.tileWidth,
                            ts.tileHeight,
                            ts.collidables.count(id_block - offset_tileset) > 0};
                }
                id_block++;
            }
        }
    }
}

void BlockTextureParser::set_skins_textures() {
    TilesetInfo tileset_skins = {"", 3, 2, 32, 32, {}, {}};
    std::unordered_map<CounterTerroristSkin, std::string> ct = {
            {SEAL_FORCE, "../assets/gfx/player/ct1.bmp"},
            {GSG_9, "../assets/gfx/player/ct2.bmp"},
            {UK_SAS, "../assets/gfx/player/ct3.bmp"},
            {GIGN, "../assets/gfx/player/ct4.bmp"}};

    std::unordered_map<TerroristSkin, std::string> tt = {
            {PHEONIX, "../assets/gfx/player/t1.bmp"},
            {L3337_KREW, "../assets/gfx/player/t2.bmp"},
            {ARTIC_AVENGER, "../assets/gfx/player/t3.bmp"},
            {GUERRILLA, "../assets/gfx/player/t4.bmp"}};

    for (const auto& pair: ct) {
        std::vector<BlockTextureInfo> sprites;
        for (int row = 0; row < tileset_skins.rows; ++row) {
            for (int col = 0; col < tileset_skins.columns; ++col) {
                sprites.push_back({pair.second, col * tileset_skins.tileWidth,
                                   row * tileset_skins.tileHeight, tileset_skins.tileWidth,
                                   tileset_skins.tileHeight, true});
            }
        }
        ct_skins[pair.first] = sprites;
    }
    for (const auto& pair: tt) {
        std::vector<BlockTextureInfo> sprites;
        for (int row = 0; row < tileset_skins.rows; ++row) {
            for (int col = 0; col < tileset_skins.columns; ++col) {
                sprites.push_back({pair.second, col * tileset_skins.tileWidth,
                                   row * tileset_skins.tileHeight, tileset_skins.tileWidth,
                                   tileset_skins.tileHeight, true});
            }
        }
        tt_skins[pair.first] = sprites;
    }
}

void BlockTextureParser::set_tileset_numbers() {
    TilesetInfo tileset_numbers = {"../assets/gfx/hud_nums.png", 1, 11, 48, 66, {}, {}};
    std::vector<HudNumbers> numbers = {ZERO, ONE,   TWO,   THREE, FOUR, FIVE,
                                       SIX,  SEVEN, EIGHT, NINE,  DP};
    // DP (:) tiene ancho 10px en el spritesheet en vez de 48
    for (size_t i = 0; i < numbers.size(); ++i) {
        auto n = numbers[i];
        bool is_dp = (n == HudNumbers::DP);

        number_textures[n] = {tileset_numbers.file,
                              static_cast<int>(i * tileset_numbers.tileWidth),
                              0,
                              is_dp ? 10 : tileset_numbers.tileWidth,
                              tileset_numbers.tileHeight,
                              false};
    }
}

void BlockTextureParser::set_symbols() {
    // por ahora cargo todas las texturas de este archivo, luego podria solo cargar las necesarias.
    TilesetInfo tileset_symbols = {"../assets/gfx/hud_symbols.png", 1, 13, 64, 64, {}, {}};
    for (int i = 0; i < tileset_symbols.columns; ++i) {
        symbol_textures[i] = {tileset_symbols.file,      i * tileset_symbols.tileWidth, 0,
                              tileset_symbols.tileWidth, tileset_symbols.tileHeight,    false};
    }
}

void BlockTextureParser::set_crosshairs() {
    TilesetInfo tileset_crosshairs = {"../assets/gfx/pointer.png", 2, 2, 46, 46, {}, {}};
    std::vector<Crosshairs> crosshairs = {GREEN, RED, YELLOW, TIME};

    int i = 0;
    for (int row = 0; row < tileset_crosshairs.rows; ++row) {
        for (int col = 0; col < tileset_crosshairs.columns; ++col) {
            crosshair_textures[crosshairs[i]] = {tileset_crosshairs.file,
                                                 col * tileset_crosshairs.tileWidth,
                                                 row * tileset_crosshairs.tileHeight,
                                                 tileset_crosshairs.tileWidth,
                                                 tileset_crosshairs.tileHeight,
                                                 false};
            i += 1;
        }
    }
}

void BlockTextureParser::set_font_and_waiting() {
    font_and_waiting_textures = {{BACKGROUND, "../assets/gfx/splash.bmp"},
                                 {FONT_WAITING, "../assets/cs_regular.ttf"},
                                 {FONT_SHOP, "../assets/gfx/fonts/korean.ttf"}};
}

void BlockTextureParser::set_gun_textures() {
    gun_textures = {{AK47_GAME, "../assets/gfx/weapons/ak47.bmp"},
                    {AK47_SHOP, "../assets/gfx/weapons/ak47_k.bmp"},
                    {AWP_GAME, "../assets/gfx/weapons/awp.bmp"},
                    {AWP_SHOP, "../assets/gfx/weapons/awp_k.bmp"},
                    {M3_GAME, "../assets/gfx/weapons/m3.bmp"},
                    {M3_SHOP, "../assets/gfx/weapons/m3_k.bmp"},
                    {GLOCK_GAME, "../assets/gfx/weapons/glock.bmp"},
                    {GLOCK_SHOP, "../assets/gfx/weapons/glock_k.bmp"},
                    {KNIFE_GAME, "../assets/gfx/weapons/knife.bmp"},
                    {KNIFE_SHOP, "../assets/gfx/weapons/knife_k.bmp"},
                    {BOMB_GAME, "../assets/gfx/weapons/bomb_sq.bmp"}};
}

void BlockTextureParser::set_editor_gun_textures() {
    editor_gun_textures = {{AK47, "../assets/gfx/weapons/ak47_k.bmp"},
                           {GLOCK, "../assets/gfx/weapons/glock_k.bmp"},
                           {M3, "../assets/gfx/weapons/m3_k.bmp"},
                           {AWP, "../assets/gfx/weapons/awp_k.bmp"}};
}
