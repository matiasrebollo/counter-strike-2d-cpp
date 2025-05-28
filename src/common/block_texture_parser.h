#ifndef BLOCK_TEXTURE_PARSER_H
#define BLOCK_TEXTURE_PARSER_H

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
};

class BlockTextureParser {
protected:
    std::unordered_map<MapObjectType, BlockTextureInfo> block_textures;

public:
    BlockTextureParser();
    BlockTextureInfo get_texture_info(MapObjectType block);
    std::vector<MapObjectType> get_keys();
};

#endif
