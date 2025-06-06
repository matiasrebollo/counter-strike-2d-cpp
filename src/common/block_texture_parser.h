#ifndef BLOCK_TEXTURE_PARSER_H
#define BLOCK_TEXTURE_PARSER_H

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "map_object.h"

struct BlockTextureInfo {
    std::string tileset_path;
    int x;
    int y;
    int width;
    int height;
    bool collidable;
};

struct TilesetInfo {
    std::string file;
    int rows;
    int columns;
    int tileWidth;
    int tileHeight;
    std::set<int> collidables;
    std::set<int> invalids;
};

class BlockTextureParser {
protected:
    std::unordered_map<int, BlockTextureInfo> block_textures;

public:
    BlockTextureParser();
    BlockTextureInfo get_texture_info(int block);
    std::vector<int> get_keys();
};

#endif
