#include "../../../mud/include/mud.h"
#include "../../../backends/sdl_backend/include/sdl_backend.h"
#include "test_utils.h"

static int callbackCalled = 0;

Mud_AppResult onKeyDown(Mud_Event* event, Mud_App* app) {
    (void)event;
    (void)app;

    callbackCalled++;
    return MUD_CONTINUE;
}

int main(void) {
    Mud_App app;

    check(Mud_init(&app, NULL) == MUD_CONTINUE);

    app.events.onKeyDown = onKeyDown;

    SDL_PumpEvents();
    SDL_FlushEvents(0, -1);
    check(Mud_handleEvents(&app) == MUD_CONTINUE);

    check(callbackCalled == 0);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);

    return 0;
}
