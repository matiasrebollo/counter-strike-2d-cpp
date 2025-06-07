#include "shop.h"

Shop::Shop(SDL2pp::Renderer& renderer, TextureManager& texture_manager,
           BlockTextureParser& texture_parser, float hud_scale):
        renderer(renderer),
        texture_manager(texture_manager),
        texture_parser(texture_parser),
        shop_rect(SDL2pp::Rect(40, 25, 400, 250)),
        open(false)

{

    // Rectangulo close
    int close_button_size = 20;
    int close_button_margin = 5;
    SDL2pp::Rect close_button_rect(
            shop_rect.x + shop_rect.w - close_button_size - close_button_margin,
            shop_rect.y + close_button_margin, close_button_size, close_button_size);
    ShopButton close_button = {close_button_rect, "", ShopButtonType::Close};
    buttons.push_back(close_button);

    // Botones de la tienda
    int button_w = 160;
    int button_h = 20;
    int spacing_y = 10;
    int base_x = shop_rect.x + 20;
    int base_y = shop_rect.y + 20;

    std::string button_texts[5] = {"AK-47", "AWP", "M3", "Balas primaria", "Balas secundaria"};

    ShopButtonType button_types[5] = {ShopButtonType::WeaponAK47, ShopButtonType::WeaponAWP,
                                      ShopButtonType::WeaponM3, ShopButtonType::AmmoPrimary,
                                      ShopButtonType::AmmoSecondary};

    for (int i = 0; i < 5; ++i) {
        int y_offset = i * (button_h + spacing_y);
        if (i >= 3) {
            y_offset += 30;  // extra spacing
        }
        SDL2pp::Rect btn_rect(base_x, base_y + y_offset, button_w, button_h);
        buttons.push_back({btn_rect, button_texts[i], button_types[i]});
    }


    // Definir el botón Open (icono tienda) que aparece fuera del shop_rect
    int shop_icon_w = 36 * hud_scale;  // según render_shop: 36 * hud_scale
    int shop_icon_h = 40 * hud_scale;
    int shop_icon_x = 480 - shop_icon_w - 5;          // CAMERA WIDTH
    int shop_icon_y = (300 / 2) - (shop_icon_h / 2);  // CAMERA HEIGHT

    open_button = {SDL2pp::Rect(shop_icon_x, shop_icon_y, shop_icon_w, shop_icon_h), "",
                   ShopButtonType::Open};
}

void Shop::render() {
    int border_thickness = 1;

    SDL2pp::Color shop_color(50, 50, 50, 200);
    SDL2pp::Color border_color(255, 255, 255, 255);
    SDL2pp::Color button_color(0, 0, 0, 200);
    SDL2pp::Color text_color(255, 255, 255);

    const std::string& font_path = texture_parser.get_fw_texture(FONT_SHOP);
    int font_size = 12;

    if (!open) {
        const BlockTextureInfo& shop_info = texture_parser.get_symbol_texture(SHOP);
        SDL2pp::Texture& shop_texture = texture_manager.get_texture(shop_info.tileset_path);
        shop_texture.SetColorMod(255, 255, 0);
        shop_texture.SetAlphaMod(190);

        SDL2pp::Rect src(shop_info.x, shop_info.y, shop_info.width, shop_info.height);
        renderer.Copy(shop_texture, src, open_button.rect);
        return;
    }

    // Render fondo de la tienda
    renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    renderer.SetDrawColor(shop_color);
    renderer.FillRect(shop_rect);

    // Borde fondo de la tienda
    renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);
    renderer.SetDrawColor(border_color);
    renderer.FillRect(SDL2pp::Rect(shop_rect.x, shop_rect.y, shop_rect.w, border_thickness));
    renderer.FillRect(SDL2pp::Rect(shop_rect.x, shop_rect.y + shop_rect.h - border_thickness,
                                   shop_rect.w, border_thickness));
    renderer.FillRect(SDL2pp::Rect(shop_rect.x, shop_rect.y, border_thickness, shop_rect.h));
    renderer.FillRect(SDL2pp::Rect(shop_rect.x + shop_rect.w - border_thickness, shop_rect.y,
                                   border_thickness, shop_rect.h));

    // Botones
    for (const ShopButton& btn: buttons) {
        if (btn.type == Open)
            continue;

        // Fondo
        renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer.SetDrawColor(button_color);
        renderer.FillRect(btn.rect);

        // Borde
        renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);
        renderer.SetDrawColor(border_color);
        renderer.FillRect(SDL2pp::Rect(btn.rect.x, btn.rect.y, btn.rect.w, border_thickness));
        renderer.FillRect(SDL2pp::Rect(btn.rect.x, btn.rect.y + btn.rect.h - border_thickness,
                                       btn.rect.w, border_thickness));
        renderer.FillRect(SDL2pp::Rect(btn.rect.x, btn.rect.y, border_thickness, btn.rect.h));
        renderer.FillRect(SDL2pp::Rect(btn.rect.x + btn.rect.w - border_thickness, btn.rect.y,
                                       border_thickness, btn.rect.h));

        // Texto (si tiene)
        if (!btn.text.empty()) {
            SDL2pp::Texture& text_tex =
                    texture_manager.get_text_texture(btn.text, font_path, font_size, text_color);
            int text_w = text_tex.GetWidth();
            int text_h = text_tex.GetHeight();

            int text_x = btn.rect.x + 10;
            int text_y = btn.rect.y + (btn.rect.h - text_h) / 2;

            renderer.Copy(text_tex, SDL2pp::NullOpt, SDL2pp::Rect(text_x, text_y, text_w, text_h));
        }

        if (btn.type == Close) {
            renderer.SetDrawColor(255, 255, 255);
            renderer.DrawLine(btn.rect.x + 4, btn.rect.y + 4, btn.rect.x + btn.rect.w - 4,
                              btn.rect.y + btn.rect.h - 4);
            renderer.DrawLine(btn.rect.x + btn.rect.w - 4, btn.rect.y + 4, btn.rect.x + 4,
                              btn.rect.y + btn.rect.h - 4);
        }
    }


    renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);
    renderer.SetDrawColor(0, 0, 0, 255);
}

std::optional<ShopButtonType> Shop::clicked_button(int x, int y) {
    SDL2pp::Point point(x, y);

    if (!open) {
        if (open_button.rect.Contains(point)) {
            open = true;
            return ShopButtonType::Open;
        }
        return std::nullopt;
    }

    for (const ShopButton& button: buttons) {
        if (button.rect.Contains(point)) {
            if (button.type == ShopButtonType::Close) {
                open = false;
                return ShopButtonType::Close;
            }
            return button.type;
        }
    }

    return std::nullopt;
}
