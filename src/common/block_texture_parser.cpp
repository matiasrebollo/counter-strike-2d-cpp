#include "block_texture_parser.h"

#include <algorithm>
#include <string>
#include <vector>

#include "map_object.h"

BlockTextureParser::BlockTextureParser(): block_textures() {
    std::vector<TilesetInfo> tilesets = {{"aztec.bmp",
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
}

BlockTextureInfo BlockTextureParser::get_texture_info(int block) { return block_textures[block]; }

std::vector<int> BlockTextureParser::get_keys() {
    std::vector<int> keys;
    keys.reserve(block_textures.size());

    std::transform(block_textures.begin(), block_textures.end(), std::back_inserter(keys),
                   [](const auto& pair) { return pair.first; });
    return keys;
}
