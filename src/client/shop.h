#ifndef SHOP_H
#define SHOP_H

#include <optional>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/block_texture_parser.h"
#include "../common/weapon_type.h"

#include "camera.h"
#include "texture_manager.h"


enum ShopButtonType { WeaponAK47, WeaponAWP, WeaponM3, AmmoPrimary, AmmoSecondary, Close, Open };

struct ShopButton {
    SDL2pp::Rect rect;
    std::string text;
    ShopButtonType type;
};

class Shop {
private:
    SDL2pp::Renderer& renderer;
    TextureManager& texture_manager;
    BlockTextureParser& texture_parser;

    std::vector<ShopButton> buttons;
    SDL2pp::Rect shop_rect;
    SDL2pp::Rect money_rect;
    SDL2pp::Rect primary_gun_rect;
    SDL2pp::Rect secondary_gun_rect;
    SDL2pp::Rect line_rect;
    SDL2pp::Point equipment_text;
    ShopButton open_button;
    bool open;

public:
    Shop(SDL2pp::Renderer& renderer, TextureManager& texture_manager,
         BlockTextureParser& texture_parser);

    /* Renderiza la tienda */
    void render(int player_money, GunType primary_gun, GunType secondary_gun);
    std::optional<ShopButtonType> clicked_button(int x, int y);
};


#endif
