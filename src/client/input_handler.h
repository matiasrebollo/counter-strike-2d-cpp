#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "../common/commands_dto.h"

#include "SDLManager.h"
#include "client_sender.h"

class InputHandler {
private:
    SDLManager& sdl;
    ClientSender sender;

    // quizas englobar logica al mandar eventos en otra clase.
    bool w = false;
    bool a = false;
    bool s = false;
    bool d = false;

    bool click_buy = false;
    bool click_attack = false;

    bool handle_quit_event(const SDL_Event& event);

    bool handle_mouse_button_down(const SDL_Event& event);
    bool handle_mouse_button_up(const SDL_Event& event);

    bool handle_keydown_event(const SDL_Event& event);
    bool handle_keyup_event(const SDL_Event& event);
    double calculate_angle_to_mouse(int mouse_x, int mouse_y) const;
    bool handle_mouse_motion_event(const SDL_Event& event);
    bool handle_shoot_event(const SDL_Event& event);
    bool handle_release_shoot_event(const SDL_Event& event);

    template <typename T>
    bool try_move(bool& flag, T dto);
    template <typename T>
    bool try_stop(bool& flag, T dto);

public:
    InputHandler(SDLManager& sdl, ClientProtocol& protocol);

    bool handle_waiting_events();
    bool handle_buy_events();
    bool handle_events();

    void start_sender();
    void close_sender_queue();
    void stop_sender();
    void join_sender();
};

#endif
