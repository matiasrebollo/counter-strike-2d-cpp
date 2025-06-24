#include "input_handler.h"

InputHandler::InputHandler(SDLManager& sdl, ClientProtocol& protocol): sdl(sdl), sender(protocol) {}

bool InputHandler::handle_quit_event(const SDL_Event& event) {
    return event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE;
}

bool InputHandler::handle_force_start(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_k && !k) {
        k = true;
        sender.add_command_to_queue(ForceStartDTO{});
        return true;
    }

    if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_k && k) {
        k = false;
        return true;
    }

    return false;
}


bool InputHandler::handle_waiting_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return false;
        if (handle_force_start(event))
            continue;
    }
    return true;
}

bool InputHandler::handle_ended_events() { return handle_waiting_events(); }

bool InputHandler::handle_between_rounds_events() { return handle_attack_events(); }

bool InputHandler::handle_weapon_switch_event(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_1:
                if (!one) {
                    sender.add_command_to_queue(EquipPrimaryDTO());
                    one = true;
                }
                return true;
            case SDLK_2:
                if (!two) {
                    sender.add_command_to_queue(EquipSecondaryDTO());
                    two = true;
                }
                return true;
            case SDLK_3:
                if (!three) {
                    sender.add_command_to_queue(EquipKnifeDTO());
                    three = true;
                }
                return true;
            case SDLK_4:
                if (!four) {
                    sender.add_command_to_queue(EquipBombDTO());
                    four = true;
                }
                return true;
        }
    }

    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_1:
                one = false;
                return true;
            case SDLK_2:
                two = false;
                return true;
            case SDLK_3:
                three = false;
                return true;
            case SDLK_4:
                four = false;
                return true;
        }
    }

    return false;
}


bool InputHandler::handle_click_shop_event(const SDL_Event& event, int money, GunType primary) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && !click_buy) {

        click_buy = true;

        int mouse_x = event.button.x;
        int mouse_y = event.button.y;

        auto opt_button = sdl.interact_button(mouse_x, mouse_y, money, primary, true);
        if (opt_button.has_value()) {
            ShopButtonType button = opt_button.value();
            switch (button) {
                case ShopButtonType::Open:
                    break;
                case ShopButtonType::Close:
                    break;
                case ShopButtonType::WeaponAK47:
                    sender.add_command_to_queue(BuyGunDTO{AK47});
                    break;
                case ShopButtonType::WeaponAWP:
                    sender.add_command_to_queue(BuyGunDTO{AWP});
                    break;
                case ShopButtonType::WeaponM3:
                    sender.add_command_to_queue(BuyGunDTO{M3});
                    break;
                case ShopButtonType::AmmoPrimary:
                    sender.add_command_to_queue(BuyAmmoDTO{true});
                    break;
                case ShopButtonType::AmmoSecondary:
                    sender.add_command_to_queue(BuyAmmoDTO{false});
                    break;
                default:
                    break;
            }
        }
        return true;
    }

    if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT && click_buy) {
        click_buy = false;
        return true;
    }

    return false;
}

bool InputHandler::handle_move_shop_event(const SDL_Event& event, int money, GunType primary) {
    if (event.type == SDL_MOUSEMOTION) {
        int mouse_x = event.motion.x;
        int mouse_y = event.motion.y;

        sdl.interact_button(mouse_x, mouse_y, money, primary, false);
        return true;
    }
    return false;
}


bool InputHandler::handle_buy_events(int money, GunType primary) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return false;
        if (handle_quit_event(event)) {
            sdl.close_shop();
            continue;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_b) {
            sdl.open_shop();
            continue;
        }
        if (handle_weapon_switch_event(event))
            continue;
        if (handle_click_shop_event(event, money, primary))
            continue;
        if (handle_move_shop_event(event, money, primary))
            continue;
    }
    return true;
}

/* Envia comando de comenzar a moverse si ya no nos estamos moviendo */
template <typename T>
bool InputHandler::try_move(bool& flag, T dto) {
    if (!flag) {
        sender.add_command_to_queue(dto);
        flag = true;
    }
    return true;
}

/* Envia comando de dejar de moverse si ya no estamos quietos */
template <typename T>
bool InputHandler::try_stop(bool& flag, T dto) {
    if (flag) {
        sender.add_command_to_queue(dto);
        flag = false;
    }
    return true;
}

/* Maneja evento de tocar una tecla de movimiento */
bool InputHandler::handle_keydown_event(const SDL_Event& event) {
    if (event.type != SDL_KEYDOWN)
        return false;

    switch (event.key.keysym.sym) {
        case SDLK_w:
            return try_move(w, MoveDTO{UP, true});
        case SDLK_a:
            return try_move(a, MoveDTO{LEFT, true});
        case SDLK_s:
            return try_move(s, MoveDTO{DOWN, true});
        case SDLK_d:
            return try_move(d, MoveDTO{RIGHT, true});
        default:
            return false;
    }
}

/* Maneja evento de soltar una tecla de movimiento */
bool InputHandler::handle_keyup_event(const SDL_Event& event) {
    if (event.type != SDL_KEYUP)
        return false;

    switch (event.key.keysym.sym) {
        case SDLK_w:
            return try_stop(w, MoveDTO{UP, false});
        case SDLK_a:
            return try_stop(a, MoveDTO{LEFT, false});
        case SDLK_s:
            return try_stop(s, MoveDTO{DOWN, false});
        case SDLK_d:
            return try_stop(d, MoveDTO{RIGHT, false});
        default:
            return false;
    }
}

/* Calcula el angulo del mouse en coords logicas al centro de la camara */
double InputHandler::calculate_angle_to_mouse(int mouse_x, int mouse_y) const {
    auto [width, height] = sdl.get_logical_size();
    float center_x = width / 2.0f;
    float center_y = height / 2.0f;
    float dx = mouse_x - center_x;
    float dy = mouse_y - center_y;
    float ang_radianes = atan2(dy, dx);
    return (ang_radianes * 180.0f / M_PI);
}

/* Maneja evento de disparo */
bool InputHandler::handle_mouse_motion_event(const SDL_Event& event) {
    if (event.type != SDL_MOUSEMOTION)
        return false;

    int mouse_x = event.motion.x;
    int mouse_y = event.motion.y;

    double angulo = calculate_angle_to_mouse(mouse_x, mouse_y);

    sender.add_command_to_queue(RotateDTO{angulo});
    return true;
}

/* Maneja evento de acción del player */
bool InputHandler::handle_shoot_event(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT &&
        !click_attack) {
        click_attack = true;
        sender.add_command_to_queue(PlayerActionDTO{true});
        return true;
    }

    if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT && click_attack) {
        click_attack = false;
        sender.add_command_to_queue(PlayerActionDTO{false});
        return true;
    }

    return false;
}

/* Maneja evento de defuseo de bomba */
bool InputHandler::handle_defuse_event(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_e && !e) {
        e = true;
        sender.add_command_to_queue(DefuseBombDTO{true});
        return true;
    }

    if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_e && e) {
        e = false;
        sender.add_command_to_queue(DefuseBombDTO{false});
        return true;
    }

    return false;
}

bool InputHandler::handle_pick_up_event(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f && !f) {
        f = true;
        sender.add_command_to_queue(PickUpItemDTO{});
        return true;
    }

    if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_f && f) {
        f = false;
        return true;
    }

    return false;
}

bool InputHandler::handle_attack_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return false;
        if (handle_weapon_switch_event(event))
            continue;
        if (handle_keydown_event(event))
            continue;
        if (handle_keyup_event(event))
            continue;
        if (handle_mouse_motion_event(event))
            continue;
        if (handle_shoot_event(event))
            continue;
        if (handle_defuse_event(event))
            continue;
        if (handle_pick_up_event(event))
            continue;
    }
    return true;
}


void InputHandler::start_sender() { sender.start(); }

void InputHandler::close_sender_queue() { sender.close_queue(); }

void InputHandler::stop_sender() { sender.stop(); }

void InputHandler::join_sender() { sender.join(); }
