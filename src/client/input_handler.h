#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "../common/game_commands_dto.h"

#include "SDLManager.h"
#include "client_sender.h"

class InputHandler {
private:
    SDLManager& sdl;
    ClientSender sender;

    // quizas englobar logica de botones/clicks en otra clase
    bool w = false;
    bool a = false;
    bool s = false;
    bool d = false;
    bool e = false;

    bool one = false;
    bool two = false;
    bool three = false;
    bool four = false;

    bool click_buy = false;
    bool click_attack = false;

    bool handle_quit_event(const SDL_Event& event);
    bool handle_weapon_switch_event(const SDL_Event& event);

    bool handle_click_shop_event(const SDL_Event& event, int money, GunType primary);
    bool handle_move_shop_event(const SDL_Event& event, int money, GunType primary);

    bool handle_keydown_event(const SDL_Event& event);
    bool handle_keyup_event(const SDL_Event& event);
    double calculate_angle_to_mouse(int mouse_x, int mouse_y) const;
    bool handle_mouse_motion_event(const SDL_Event& event);
    bool handle_shoot_event(const SDL_Event& event);
    bool handle_defuse_event(const SDL_Event& event);

    template <typename T>
    bool try_move(bool& flag, T dto);
    template <typename T>
    bool try_stop(bool& flag, T dto);

public:
    InputHandler(SDLManager& sdl, ClientProtocol& protocol);

    /* Maneja eventos del cliente en la fase waiting */
    bool handle_waiting_events();
    /* Maneja eventos del cliente en la fase de compra */
    bool handle_buy_events(int money, GunType primary);
    /* Maneja eventos del cliente en la fase de juego */
    bool handle_attack_events();

    bool handle_ended_events();
    bool handle_between_rounds_events();

    void start_sender();
    void close_sender_queue();
    void stop_sender();
    void join_sender();
};

#endif
