#include "shop.h"

#include <utility>

Shop::Shop(SDL2pp::Renderer& renderer, TextureManager& texture_manager,
           BlockTextureParser& texture_parser):
        renderer(renderer),
        texture_manager(texture_manager),
        texture_parser(texture_parser),
        shop_rect(SDL2pp::Rect(53, 33, 533, 333)),
        open(false)

{

    // Rectangulo close
    int close_button_size = 27;
    int close_button_margin = 7;
    SDL2pp::Rect close_button_rect(
            shop_rect.x + shop_rect.w - close_button_size - close_button_margin,
            shop_rect.y + close_button_margin, close_button_size, close_button_size);
    ShopButton close_button = {close_button_rect, "", ShopButtonType::Close};
    buttons.push_back(close_button);

    // Rectangulo money
    int money_rect_w = 80;
    int money_rect_h = 27;
    int spacing_from_close = 10;

    money_rect = SDL2pp::Rect(close_button_rect.x + close_button_rect.w - money_rect_w,
                              close_button_rect.y + close_button_rect.h + spacing_from_close,
                              money_rect_w, money_rect_h);

    // Rectangulos equipment
    int eq_rect_w_primary = 80;
    int eq_rect_w_secondary = 64;
    int eq_rect_h = 32;
    int eq_spacing_x = 20;

    int eq_base_y = shop_rect.y + shop_rect.h - 27 - eq_rect_h;
    int eq_base_x = shop_rect.x + 27;

    primary_gun_rect = SDL2pp::Rect(eq_base_x, eq_base_y, eq_rect_w_primary, eq_rect_h);
    secondary_gun_rect = SDL2pp::Rect(eq_base_x + eq_rect_w_primary + eq_spacing_x, eq_base_y,
                                      eq_rect_w_secondary, eq_rect_h);
    // knife_rect = SDL2pp::Rect(eq_base_x + 2 * (eq_rect_w + eq_spacing_x), eq_base_y, eq_rect_w,
    // eq_rect_h);

    // Linea divisoria
    int line_margin = 20;
    int line_thickness = 1;
    line_rect = SDL2pp::Rect(shop_rect.x + line_margin, eq_base_y - 20,
                             shop_rect.w - 2 * line_margin, line_thickness);

    // Texto equipamiento
    equipment_text = SDL2pp::Point(shop_rect.x + 27, line_rect.y + 2);

    // Botones de la tienda
    int button_w = 213;
    int button_h = 27;
    int spacing_y = 13;
    int base_x = shop_rect.x + 27;
    int base_y = shop_rect.y + 27;

    std::string button_texts[5] = {"AK 47", "AWP", "M3", "Primary ammo", "Secondary ammo"};

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


    // Boton open
    int shop_icon_w = 36;
    int shop_icon_h = 40;
    int shop_icon_x = CAMERA_WIDTH - shop_icon_w - 5;
    int shop_icon_y = (CAMERA_HEIGHT / 2) - (shop_icon_h / 2);

    open_button = {SDL2pp::Rect(shop_icon_x, shop_icon_y, shop_icon_w, shop_icon_h), "",
                   ShopButtonType::Open};
}

void Shop::render(int player_money, GunType primary_gun, GunType secondary_gun) {
    int border_thickness = 1;

    SDL2pp::Color shop_color(50, 50, 50, 200);
    SDL2pp::Color border_color(255, 255, 255, 255);
    SDL2pp::Color button_color(0, 0, 0, 200);
    SDL2pp::Color text_color(255, 255, 255);

    const std::string& font_path = texture_parser.get_fw_texture(FONT_WAITING);
    int font_size = 16;

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

    // Rectangulo dinero
    renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    renderer.SetDrawColor(button_color);
    renderer.FillRect(money_rect);

    // Borde rectangulo dinero
    renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);
    renderer.SetDrawColor(border_color);
    renderer.FillRect(SDL2pp::Rect(money_rect.x, money_rect.y, money_rect.w, border_thickness));
    renderer.FillRect(SDL2pp::Rect(money_rect.x, money_rect.y + money_rect.h - border_thickness,
                                   money_rect.w, border_thickness));
    renderer.FillRect(SDL2pp::Rect(money_rect.x, money_rect.y, border_thickness, money_rect.h));
    renderer.FillRect(SDL2pp::Rect(money_rect.x + money_rect.w - border_thickness, money_rect.y,
                                   border_thickness, money_rect.h));

    // Texto dinero
    std::string money_str = std::to_string(player_money);  // suponiendo que recibís 'money'
    std::string dollar = "$";

    SDL2pp::Texture& dollar_tex =
            texture_manager.get_text_texture(dollar, font_path, font_size, text_color);
    SDL2pp::Texture& money_tex =
            texture_manager.get_text_texture(money_str, font_path, font_size, text_color);

    int dollar_x = money_rect.x + 4;
    int dollar_y = money_rect.y + (money_rect.h - dollar_tex.GetHeight()) / 2;

    int money_x = money_rect.x + money_rect.w - money_tex.GetWidth() - 4;
    int money_y = money_rect.y + (money_rect.h - money_tex.GetHeight()) / 2;

    renderer.Copy(dollar_tex, SDL2pp::NullOpt,
                  SDL2pp::Rect(dollar_x, dollar_y, dollar_tex.GetWidth(), dollar_tex.GetHeight()));

    renderer.Copy(money_tex, SDL2pp::NullOpt,
                  SDL2pp::Rect(money_x, money_y, money_tex.GetWidth(), money_tex.GetHeight()));


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

    // Equipamiento
    renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);

    // Linea divisoria
    renderer.SetDrawColor(border_color);
    renderer.FillRect(line_rect);

    // Texto equipment
    std::string equipment_str = "Equipment";
    int small_font_size = 12;
    SDL2pp::Texture& equipment_tex =
            texture_manager.get_text_texture(equipment_str, font_path, small_font_size, text_color);

    renderer.Copy(equipment_tex, SDL2pp::NullOpt,
                  SDL2pp::Rect(equipment_text.x, equipment_text.y, equipment_tex.GetWidth(),
                               equipment_tex.GetHeight()));

    std::vector<std::pair<GunType, SDL2pp::Rect>> equipment = {
            {primary_gun, primary_gun_rect}, {secondary_gun, secondary_gun_rect},
            //{KNIFE, knife_rect}
    };

    for (const auto& [gun, rect]: equipment) {
        // Rectangulo
        renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer.SetDrawColor(0, 0, 0);
        renderer.FillRect(rect);

        // Borde
        renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);
        renderer.SetDrawColor(border_color);
        renderer.FillRect(SDL2pp::Rect(rect.x, rect.y, rect.w, border_thickness));
        renderer.FillRect(
                SDL2pp::Rect(rect.x, rect.y + rect.h - border_thickness, rect.w, border_thickness));
        renderer.FillRect(SDL2pp::Rect(rect.x, rect.y, border_thickness, rect.h));
        renderer.FillRect(
                SDL2pp::Rect(rect.x + rect.w - border_thickness, rect.y, border_thickness, rect.h));

        // Textura del arma
        if (gun != NONE) {
            GunSprites sprite;
            switch (gun) {
                case GLOCK:
                    sprite = GLOCK_SHOP;
                    break;
                case AK47:
                    sprite = AK47_SHOP;
                    break;
                case M3:
                    sprite = M3_SHOP;
                    break;
                case AWP:
                    sprite = AWP_SHOP;
                    break;
                default:
                    continue;
            }

            std::string weapon_path = texture_parser.get_gun_texture(sprite);
            SDL2pp::Texture& gun_tex = texture_manager.get_texture(weapon_path);
            gun_tex.SetAlphaMod(200);

            int margin_x = 10;
            int margin_y = 4;

            SDL2pp::Rect dst(rect.x + margin_x, rect.y + margin_y, rect.w - 2 * margin_x,
                             rect.h - 2 * margin_y);

            renderer.Copy(gun_tex, SDL2pp::NullOpt, dst);
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
