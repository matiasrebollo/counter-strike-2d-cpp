#include "block_texture_parser.h"

#include <algorithm>
#include <string>
#include <vector>

#include "map_object.h"

BlockTextureParser::BlockTextureParser(): block_textures(), backgrounds_paths() {
    /*backgrounds_paths = {{AZTEC_BACKGROUND, "../assets/gfx/backgrounds/aztec.png"},
                         {DUST_BACKGROUND, "../assets/gfx/backgrounds/dust.png"},
                         {GRASS_BACKGROUND, "../assets/gfx/backgrounds/gras1.jpg"}};*/

    backgrounds_paths[AZTEC_BACKGROUND] = "../assets/gfx/backgrounds/aztec.png";
    backgrounds_paths[DUST_BACKGROUND] = "../assets/gfx/backgrounds/dust.png";
    backgrounds_paths[GRASS_BACKGROUND] = "../assets/gfx/backgrounds/gras1.jpg";

    std::vector<TilesetInfo> tilesets = {{"../assets/gfx/tiles/aztec.bmp",
                                          6,
                                          16,
                                          32,
                                          32,
                                          {1, 2, 3, 4, 5, 6, 7, 8, 47, 46, 45, 93, 92, 88, 87},
                                          {0, 9, 10, 11, 12, 13, 14, 15, 95, 94, 91, 90, 89, 73}}};

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

    // por ahora cargo todas las texturas de este archivo, luego podria solo cargar las necesarias.
    TilesetInfo tileset_symbols = {"../assets/gfx/hud_symbols.png", 1, 13, 64, 64, {}, {}};
    for (int i = 0; i < tileset_symbols.columns; ++i) {
        symbol_textures[i] = {tileset_symbols.file,      i * tileset_symbols.tileWidth, 0,
                              tileset_symbols.tileWidth, tileset_symbols.tileHeight,    false};
    }

    font_and_waiting_textures[BACKGROUND] = "../assets/gfx/splash.bmp";
    font_and_waiting_textures[FONT_WAITING] = "../assets/cs_regular.ttf";
    font_and_waiting_textures[FONT_SHOP] = "../assets/gfx/fonts/korean.ttf";
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
