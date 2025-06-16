#ifndef SHOP_H
#define SHOP_H

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/block_texture_parser.h"
#include "../common/game_info_dto.h"
#include "../common/weapon_type.h"

#include "camera.h"
#include "texture_manager.h"


enum ShopButtonType {
    WeaponAK47,
    WeaponAWP,
    WeaponM3,
    AmmoPrimary,
    AmmoSecondary,
    Close,
    Open,
    None
};

struct ShopButton {
    SDL2pp::Rect rect;
    std::string text;
    ShopButtonType type;
    int price = 0;
    GunType weapon_type = NONE;
};

class Shop {
private:
    SDL2pp::Renderer& renderer;
    SDL2pp::Mixer& mixer;
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

    std::unordered_map<GunType, int> ammo_by_clip;

    bool highlight_money = false;
    bool highlight_primary = false;
    ShopButtonType touched_button_type = None;
    ShopButtonType last_touched_button_type = None;

public:
    Shop(SDL2pp::Renderer& renderer, SDL2pp::Mixer& mixer, TextureManager& texture_manager,
         BlockTextureParser& texture_parser);

    /* Setea los precios y cantidades de cada cosa de la tienda, al comenzar la partida */
    void set_shop_info(const ShopInfoDTO& info);

    /* Renderiza la tienda */
    void render(int player_money, GunType primary_gun, GunType secondary_gun);

    /* Devuelve el boton clickeado si x e y estan dentro, o nada si no esta dentro
     * o si no alcanza el dinero o si ya se posee esa arma.
     */
    std::optional<ShopButtonType> interact_button(int x, int y, int money, GunType primary_gun,
                                                  bool click);
};


#endif
