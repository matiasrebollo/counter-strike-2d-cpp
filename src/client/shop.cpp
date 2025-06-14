#include "shop.h"

#include <utility>

Shop::Shop(SDL2pp::Renderer& renderer, SDL2pp::Mixer& mixer, TextureManager& texture_manager,
           BlockTextureParser& texture_parser):
        renderer(renderer),
        mixer(mixer),
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

void Shop::set_shop_info(const ShopInfoDTO& info) {
    ammo_by_clip = info.ammo_by_clip;

    for (auto& button: buttons) {
        switch (button.type) {
            case WeaponAK47:
                button.weapon_type = AK47;
                button.price = info.prices.at(AK47);
                break;
            case WeaponAWP:
                button.weapon_type = AWP;
                button.price = info.prices.at(AWP);
                break;
            case WeaponM3:
                button.weapon_type = M3;
                button.price = info.prices.at(M3);
                break;
            default:
                break;  // AmmoPrimary y AmmoSecondary se resuelven en render()
        }
    }
}

void Shop::render(int player_money, GunType primary_gun, GunType secondary_gun) {
    int border_thickness = 1;

    SDL2pp::Color shop_color(50, 50, 50, 200);
    SDL2pp::Color border_color(255, 255, 255, 255);
    SDL2pp::Color highlight_color(255, 255, 0, 255);
    SDL2pp::Color button_color(0, 0, 0, 200);
    SDL2pp::Color text_color(255, 255, 255);

    const std::string& font_path = texture_parser.get_fw_texture(FONT_WAITING);
    int font_size = 16;
    int small_font_size = 12;

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
    SDL2pp::Rect draw_money_rect = money_rect;
    if (highlight_money) {
        int expand = 2;
        draw_money_rect.x -= expand;
        draw_money_rect.y -= expand;
        draw_money_rect.w += 2 * expand;
        draw_money_rect.h += 2 * expand;
    }

    renderer.FillRect(draw_money_rect);

    // Borde rectangulo dinero
    renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);
    SDL2pp::Color money_border = highlight_money ? highlight_color : border_color;
    renderer.SetDrawColor(money_border);
    renderer.FillRect(SDL2pp::Rect(draw_money_rect.x, draw_money_rect.y, draw_money_rect.w,
                                   border_thickness));
    renderer.FillRect(SDL2pp::Rect(draw_money_rect.x,
                                   draw_money_rect.y + draw_money_rect.h - border_thickness,
                                   draw_money_rect.w, border_thickness));
    renderer.FillRect(SDL2pp::Rect(draw_money_rect.x, draw_money_rect.y, border_thickness,
                                   draw_money_rect.h));
    renderer.FillRect(SDL2pp::Rect(draw_money_rect.x + draw_money_rect.w - border_thickness,
                                   draw_money_rect.y, border_thickness, draw_money_rect.h));

    // Texto dinero
    std::string money_str = std::to_string(player_money);
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
        if (btn.type == ShopButtonType::Open)
            continue;


        // Fondo
        renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer.SetDrawColor(button_color);

        SDL2pp::Rect draw_rect = btn.rect;
        if (btn.type == touched_button_type) {
            int expand = 2;
            draw_rect.x -= expand;
            draw_rect.y -= expand;
            draw_rect.w += 2 * expand;
            draw_rect.h += 2 * expand;
        }
        renderer.FillRect(draw_rect);

        // Borde
        renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);
        renderer.SetDrawColor(border_color);
        renderer.FillRect(SDL2pp::Rect(draw_rect.x, draw_rect.y, draw_rect.w, border_thickness));
        renderer.FillRect(SDL2pp::Rect(draw_rect.x, draw_rect.y + draw_rect.h - border_thickness,
                                       draw_rect.w, border_thickness));
        renderer.FillRect(SDL2pp::Rect(draw_rect.x, draw_rect.y, border_thickness, draw_rect.h));
        renderer.FillRect(SDL2pp::Rect(draw_rect.x + draw_rect.w - border_thickness, draw_rect.y,
                                       border_thickness, draw_rect.h));

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

        // Precio
        int price = 0;
        if (btn.type == ShopButtonType::AmmoPrimary || btn.type == ShopButtonType::AmmoSecondary) {
            price = 50;  // hardcodeado por ahora
        } else if (btn.weapon_type != NONE) {
            price = btn.price;
        }

        if (price > 0) {
            std::string price_str = "$" + std::to_string(price);
            SDL2pp::Color grey_color(180, 180, 180);
            SDL2pp::Texture& price_tex = texture_manager.get_text_texture(
                    price_str, font_path, small_font_size, grey_color);

            int price_x = btn.rect.x + btn.rect.w - price_tex.GetWidth() - 6;
            int price_y = btn.rect.y + btn.rect.h - price_tex.GetHeight() - 4;

            renderer.Copy(
                    price_tex, SDL2pp::NullOpt,
                    SDL2pp::Rect(price_x, price_y, price_tex.GetWidth(), price_tex.GetHeight()));
        }

        // Cantidad de balas
        if ((btn.type == ShopButtonType::AmmoPrimary && primary_gun != NONE) ||
            (btn.type == ShopButtonType::AmmoSecondary && secondary_gun != NONE)) {
            int ammo = 0;

            if (btn.type == ShopButtonType::AmmoPrimary) {
                ammo = ammo_by_clip.at(primary_gun);
            } else {
                ammo = ammo_by_clip.at(secondary_gun);
            }

            std::string ammo_str = "+" + std::to_string(ammo);
            SDL2pp::Color grey_color(180, 180, 180);
            SDL2pp::Texture& ammo_tex = texture_manager.get_text_texture(
                    ammo_str, font_path, small_font_size, grey_color);

            int ammo_x = btn.rect.x + btn.rect.w - ammo_tex.GetWidth() - 6;
            int ammo_y = btn.rect.y + 4;

            renderer.Copy(ammo_tex, SDL2pp::NullOpt,
                          SDL2pp::Rect(ammo_x, ammo_y, ammo_tex.GetWidth(), ammo_tex.GetHeight()));
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
    int equipment_font_size = 12;
    SDL2pp::Texture& equipment_tex = texture_manager.get_text_texture(
            equipment_str, font_path, equipment_font_size, text_color);

    renderer.Copy(equipment_tex, SDL2pp::NullOpt,
                  SDL2pp::Rect(equipment_text.x, equipment_text.y, equipment_tex.GetWidth(),
                               equipment_tex.GetHeight()));

    std::vector<std::pair<GunType, SDL2pp::Rect>> equipment = {
            {primary_gun, primary_gun_rect},
            {secondary_gun, secondary_gun_rect},
    };

    for (const auto& [gun, rect]: equipment) {
        // Rectangulo
        renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer.SetDrawColor(0, 0, 0);
        SDL2pp::Rect draw_rect = rect;
        if (highlight_primary) {
            int expand = 2;
            draw_rect.x -= expand;
            draw_rect.y -= expand;
            draw_rect.w += 2 * expand;
            draw_rect.h += 2 * expand;
        }
        renderer.FillRect(draw_rect);

        // Borde
        renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);
        SDL2pp::Color equipment_border = highlight_primary ? highlight_color : border_color;
        renderer.SetDrawColor(equipment_border);
        renderer.FillRect(SDL2pp::Rect(draw_rect.x, draw_rect.y, draw_rect.w, border_thickness));
        renderer.FillRect(SDL2pp::Rect(draw_rect.x, draw_rect.y + draw_rect.h - border_thickness,
                                       draw_rect.w, border_thickness));
        renderer.FillRect(SDL2pp::Rect(draw_rect.x, draw_rect.y, border_thickness, draw_rect.h));
        renderer.FillRect(SDL2pp::Rect(draw_rect.x + draw_rect.w - border_thickness, draw_rect.y,
                                       border_thickness, draw_rect.h));

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
    highlight_money = false;
    highlight_primary = false;
}

std::optional<ShopButtonType> Shop::interact_button(int x, int y, int money, GunType primary_gun,
                                                    bool click) {
    SDL2pp::Point point(x, y);
    touched_button_type = ShopButtonType::None;

    if (!open) {
        if (open_button.rect.Contains(point) && click) {
            open = true;
            std::string path = texture_parser.get_sound_path(OPEN_SHOP);
            SDL2pp::Chunk& sound = texture_manager.get_sound(path);
            mixer.PlayChannel(-1, sound);

            return ShopButtonType::Open;
        }
        return std::nullopt;
    }

    for (const ShopButton& button: buttons) {
        if (button.rect.Contains(point)) {

            touched_button_type = button.type;

            if (!click) {
                if (button.type == ShopButtonType::Close)
                    return std::nullopt;

                if (touched_button_type != last_touched_button_type) {
                    last_touched_button_type = touched_button_type;
                    std::string path = texture_parser.get_sound_path(MOVE_SELECT);
                    SDL2pp::Chunk& sound = texture_manager.get_sound(path);
                    mixer.PlayChannel(-1, sound);
                }
                return std::nullopt;
            }

            if (button.type == ShopButtonType::Close) {
                open = false;

                std::string path = texture_parser.get_sound_path(CLOSE_SHOP);
                SDL2pp::Chunk& sound = texture_manager.get_sound(path);
                mixer.PlayChannel(-1, sound);

                return ShopButtonType::Close;
            }

            // quizas un if que incluya ambas condiciones, para setear los dos true
            // si queres comprar un arma que ya tenes y ADEMAS no te alcanza

            if (money < button.price) {
                highlight_money = true;

                std::string path = texture_parser.get_sound_path(DENY_SELECT);
                SDL2pp::Chunk& sound = texture_manager.get_sound(path);
                mixer.PlayChannel(-1, sound);

                return std::nullopt;
            }

            if ((button.type == WeaponAK47 || button.type == WeaponAWP ||
                 button.type == WeaponM3) &&
                button.weapon_type == primary_gun) {
                highlight_primary = true;

                std::string path = texture_parser.get_sound_path(DENY_SELECT);
                SDL2pp::Chunk& sound = texture_manager.get_sound(path);
                mixer.PlayChannel(-1, sound);
                return std::nullopt;
            }

            std::string path = texture_parser.get_sound_path(SELECT);
            SDL2pp::Chunk& sound = texture_manager.get_sound(path);
            mixer.PlayChannel(-1, sound);

            return button.type;
        }
    }

    return std::nullopt;
}
