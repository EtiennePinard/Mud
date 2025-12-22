#include "../../../mud/include/mud.h"
#include "../../../backends/sdl_backend/include/sdl_backend.h"
#include "test_utils.h"

static int keyDownCalled = 0;

Mud_AppResult onKeyDown(Mud_Event* event, Mud_App* app) {
    (void)app;

    keyDownCalled++;

    check(event->type == MUD_EVENT_KEY_DOWN);
    check(event->key.scancode == SDL_SCANCODE_A);
    check(event->key.keycode == SDLK_A);

    return MUD_CONTINUE;
}

int main(void) {
    Mud_App app;

    check(Mud_init(&app, NULL) == MUD_CONTINUE);

    SDL_Event event;

    event.type = SDL_EVENT_KEY_DOWN;
    event.key.scancode = SDL_SCANCODE_A;
    event.key.key = SDLK_A;

    SDL_PumpEvents();
    SDL_FlushEvents(0, -1);
    SDL_PushEvent(&event);

    app.events.onKeyDown = onKeyDown;

    check(Mud_handleEvents(&app) == MUD_CONTINUE);

    check(keyDownCalled == 0);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);

    return 0;
}
