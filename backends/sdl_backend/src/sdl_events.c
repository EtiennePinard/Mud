#include "sdl_internalState.h"

Mud_AppResult MudBackend_popEvent(Mud_Event* outEvent) {
    SDL_Event event;
    SDL_PollEvent(&event);
    Mud_AppResult appResult = MUD_CONTINUE;
    switch (event.type) {
    case SDL_EVENT_QUIT:
        outEvent->type = MUD_EVENT_QUIT;
        break;
    case SDL_EVENT_MOUSE_MOTION:
        outEvent->type = MUD_EVENT_MOUSE_MOVE;
        outEvent->mouseMove.x = event.motion.x;
        outEvent->mouseMove.y = event.motion.y;
        outEvent->mouseMove.pressedButtonFlag = event.motion.state;
        // We "normalize" the pressedButtonFlag so that we stay within the
        // buttons Mud defines
        outEvent->mouseMove.pressedButtonFlag &=
            (MUD_LEFT_BUTTON_MASK | MUD_RIGHT_BUTTON_MASK |
             MUD_MIDDLE_BUTTON_MASK);
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
            outEvent->type = MUD_EVENT_MOUSE_BUTTON_UP;
        else
            outEvent->type = MUD_EVENT_MOUSE_BUTTON_DOWN;

        outEvent->mouseButton.clicks = event.button.clicks;
        if (event.button.button == SDL_BUTTON_LEFT)
            outEvent->mouseButton.mouseButtonIndex = MUD_LEFT_MOUSE_BUTTON;
        else if (event.button.button == SDL_BUTTON_RIGHT)
            outEvent->mouseButton.mouseButtonIndex = MUD_RIGHT_MOUSE_BUTTON;
        else if (event.button.button == SDL_BUTTON_MIDDLE)
            outEvent->mouseButton.mouseButtonIndex = MUD_MIDDLE_MOUSE_BUTTON;

        outEvent->mouseButton.x = event.button.x;
        outEvent->mouseButton.y = event.button.y;
        break;

    case SDL_EVENT_MOUSE_WHEEL:
        outEvent->type = MUD_EVENT_MOUSE_WHEEL;
        outEvent->mouseWheel = (Mud_MouseWheelEvent){
            .x = event.wheel.x,
            .y = event.wheel.y,
            .direction = event.wheel.direction == SDL_MOUSEWHEEL_NORMAL
                             ? MUD_MOUSE_WHEEL_NORMAL
                             : MUD_MOUSE_WHEEL_FLIPPED,
        };
        break;

    case SDL_EVENT_WINDOW_RESIZED:
        outEvent->type = MUD_EVENT_WINDOW_RESIZE;
        outEvent->windowResize = (Mud_WindowResizeEvent){
            .width = event.window.data1,
            .height = event.window.data2,
        };
        break;
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        if (event.type == SDL_EVENT_KEY_DOWN)
            outEvent->type = MUD_EVENT_KEY_DOWN;
        else
            outEvent->type = MUD_EVENT_KEY_UP;
        outEvent->key = (Mud_KeyEvent){
            .scancode = event.key.scancode,
            .keycode = event.key.key,
        };
        break;
    default:
        outEvent->type = MUD_EVENT_NONE;
        break;
    }
    return appResult;
}
