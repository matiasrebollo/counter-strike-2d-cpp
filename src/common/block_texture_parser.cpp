#include "block_texture_parser.h"

#include <algorithm>
#include <string>
#include <vector>

#include "map_object.h"

BlockTextureParser::BlockTextureParser(): block_textures() {
    std::vector<TilesetInfo> tilesets = {{"gfx/tiles/aztec.bmp",
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
    std::unordered_map<CounterTerroristSkin, std::string> ct = {{SEAL_FORCE, "gfx/player/ct1.bmp"},
                                                                {GSG_9, "gfx/player/ct2.bmp"},
                                                                {UK_SAS, "gfx/player/ct3.bmp"},
                                                                {GIGN, "gfx/player/ct4.bmp"}};

    std::unordered_map<TerroristSkin, std::string> tt = {{PHEONIX, "gfx/player/t1.bmp"},
                                                         {L3337_KREW, "gfx/player/t2.bmp"},
                                                         {ARTIC_AVENGER, "gfx/player/t3.bmp"},
                                                         {GUERRILLA, "gfx/player/t4.bmp"}};

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

    TilesetInfo tileset_numbers = {"gfx/hud_nums.png", 1, 11, 48, 66, {}, {}};
    std::vector<HudNumbers> numbers = {ZERO, ONE,   TWO,   THREE, FOUR, FIVE,
                                       SIX,  SEVEN, EIGHT, NINE,  DP};
    int i = 0;
    for (const auto& n: numbers) {
        if (n == DP) {
            number_textures[n] = {tileset_numbers.file,
                                  i * tileset_numbers.tileWidth,
                                  0,
                                  10,
                                  tileset_numbers.tileHeight,
                                  false};
        } else {
            number_textures[n] = {tileset_numbers.file,      i * tileset_numbers.tileWidth, 0,
                                  tileset_numbers.tileWidth, tileset_numbers.tileHeight,    false};
        }
        i++;
    }
}

BlockTextureInfo BlockTextureParser::get_texture_info(int block) { return block_textures[block]; }

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

std::vector<int> BlockTextureParser::get_keys() {
    std::vector<int> keys;
    keys.reserve(block_textures.size());

    std::transform(block_textures.begin(), block_textures.end(), std::back_inserter(keys),
                   [](const auto& pair) { return pair.first; });
    return keys;
}
