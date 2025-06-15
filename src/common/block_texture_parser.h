#ifndef BLOCK_TEXTURE_PARSER_H
#define BLOCK_TEXTURE_PARSER_H

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "map_object.h"
#include "sdl_helpers.h"
#include "skins.h"
#include "weapon_type.h"

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
private:
    void set_backgrounds_paths();
    void set_blocks_textures();
    void set_skins_textures();
    void set_tileset_numbers();
    void set_symbols();
    void set_crosshairs();
    void set_font_and_waiting();
    void set_gun_textures();
    void set_editor_gun_textures();

protected:
    std::unordered_map<int, BlockTextureInfo> block_textures;
    std::unordered_map<CounterTerroristSkin, std::vector<BlockTextureInfo>> ct_skins;
    std::unordered_map<TerroristSkin, std::vector<BlockTextureInfo>> tt_skins;
    std::unordered_map<HudNumbers, BlockTextureInfo> number_textures;
    std::unordered_map<int, BlockTextureInfo> symbol_textures;
    std::unordered_map<FontsAndBackground, std::string> font_and_waiting_textures;
    std::unordered_map<Crosshairs, BlockTextureInfo> crosshair_textures;
    std::unordered_map<GunSprites, std::string> gun_textures;
    std::unordered_map<Background, std::string> backgrounds_paths;
    std::unordered_map<GunType, std::string> editor_gun_textures;

public:
    BlockTextureParser();
    // deberia ser const&
    const BlockTextureInfo& get_texture_info(int block);
    const BlockTextureInfo& get_ct_texture(CounterTerroristSkin skin, Position sprite_index) const;
    const BlockTextureInfo& get_tt_texture(TerroristSkin skin, Position sprite_index) const;
    const BlockTextureInfo& get_number_texture(HudNumbers sprite) const;
    const BlockTextureInfo& get_symbol_texture(HudSymbols symbol) const;
    const std::string& get_fw_texture(FontsAndBackground fw) const;
    const BlockTextureInfo& get_crosshair_texture(Crosshairs crosshair) const;
    const std::string& get_gun_texture(GunSprites gun) const;
    const std::vector<int> get_blocks_keys();
    const std::vector<Background> get_backgrounds();
    const std::string& get_background_path(const Background& background);
    const std::string& get_editor_gun_texture(const GunType& gun);
};

#endif
