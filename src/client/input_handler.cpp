#include "input_handler.h"

InputHandler::InputHandler(SDLManager& sdl, ClientProtocol& protocol): sdl(sdl), sender(protocol) {}


template <typename T>
bool InputHandler::try_move(bool& flag, T dto) {
    if (!flag) {
        sender.add_command_to_queue(dto);
        flag = true;
    }
    return true;
}

template <typename T>
bool InputHandler::try_stop(bool& flag, T dto) {
    if (flag) {
        sender.add_command_to_queue(dto);
        flag = false;
    }
    return true;
}


bool InputHandler::handle_quit_event(const SDL_Event& event) {
    return (event.type == SDL_QUIT) ||
           (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE);
}

bool InputHandler::handle_keydown_event(const SDL_Event& event) {
    if (event.type != SDL_KEYDOWN)
        return false;

    switch (event.key.keysym.sym) {
        case SDLK_w:
            return try_move(w, MoveUpDTO{});
        case SDLK_a:
            return try_move(a, MoveLeftDTO{});
        case SDLK_s:
            return try_move(s, MoveDownDTO{});
        case SDLK_d:
            return try_move(d, MoveRightDTO{});
        default:
            return false;
    }
}

bool InputHandler::handle_keyup_event(const SDL_Event& event) {
    if (event.type != SDL_KEYUP)
        return false;

    switch (event.key.keysym.sym) {
        case SDLK_w:
            return try_stop(w, MoveUpDTO{});
        case SDLK_a:
            return try_stop(a, MoveLeftDTO{});
        case SDLK_s:
            return try_stop(s, MoveDownDTO{});
        case SDLK_d:
            return try_stop(d, MoveRightDTO{});
        default:
            return false;
    }
}

bool InputHandler::handle_mouse_motion_event(const SDL_Event& event) {
    if (event.type != SDL_MOUSEMOTION)
        return false;

    int mouse_x = event.motion.x;
    int mouse_y = event.motion.y;

    double angulo = sdl.calculate_angle_to_mouse(mouse_x, mouse_y);
    sender.add_command_to_queue(RotateDTO{angulo});
    return true;
}


bool InputHandler::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (handle_quit_event(event))
            return false;
        if (handle_keydown_event(event))
            continue;
        if (handle_keyup_event(event))
            continue;
        if (handle_mouse_motion_event(event))
            continue;
    }
    return true;
}


void InputHandler::start_sender() { sender.start(); }

void InputHandler::stop_sender() { sender.stop(); }

void InputHandler::close_sender_queue() { sender.close_queue(); }

void InputHandler::join_sender() { sender.join(); }
