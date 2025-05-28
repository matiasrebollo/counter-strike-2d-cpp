#include "block_texture_parser.h"

#include <algorithm>
#include <string>
#include <vector>

#include "map_object.h"

BlockTextureParser::BlockTextureParser():
        block_textures({{AZTEC_STONE_WALL_1, {"aztec.bmp", 32, 0, 32, 32}},
                        {AZTEC_STONE_WALL_2, {"aztec.bmp", 64, 0, 32, 32}},
                        {AZTEC_STONE_WALL_3, {"aztec.bmp", 96, 0, 32, 32}},
                        {AZTEC_STONE_WALL_4, {"aztec.bmp", 128, 0, 32, 32}},
                        {AZTEC_STONE_WALL_5, {"aztec.bmp", 160, 0, 32, 32}},
                        {AZTEC_STONE_WALL_6, {"aztec.bmp", 192, 0, 32, 32}},
                        {AZTEC_STONE_WALL_7, {"aztec.bmp", 224, 0, 32, 32}},
                        {AZTEC_STONE_WALL_8, {"aztec.bmp", 256, 0, 32, 32}}}) {}

BlockTextureInfo BlockTextureParser::get_texture_info(MapObjectType block) {
    return block_textures[block];
}

std::vector<MapObjectType> BlockTextureParser::get_keys() {
    std::vector<MapObjectType> keys;
    keys.reserve(block_textures.size());

    std::transform(block_textures.begin(), block_textures.end(), std::back_inserter(keys),
                   [](const auto& pair) { return pair.first; });
    return keys;
}
