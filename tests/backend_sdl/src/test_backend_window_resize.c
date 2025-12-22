#include "../../../mud/include/mud.h"
#include "../../../backends/sdl_backend/include/sdl_backend.h"
#include "test_utils.h"

static int windowResizeCalled = 0;

Mud_AppResult onWindowResize(Mud_Event* event, Mud_App* app) {
    (void)app;

    windowResizeCalled++;

    check(event->type == MUD_EVENT_WINDOW_RESIZE);
    check(event->windowResize.height == 1280);
    check(event->windowResize.width == 720);

    return MUD_CONTINUE;
}

int main(void) {
    Mud_App app;

    check(Mud_init(&app, NULL) == MUD_CONTINUE);
    app.events.onWindowResize = onWindowResize;

    SDL_Event event;

    event.type = SDL_EVENT_WINDOW_RESIZED;
    event.window.data1 = 1280;
    event.window.data2 = 720;

    SDL_PumpEvents();
    SDL_FlushEvents(0, -1);
    SDL_PushEvent(&event);

    check(Mud_handleEvents(&app) == MUD_CONTINUE);

    check(windowResizeCalled == 0);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);

    return 0;
}
