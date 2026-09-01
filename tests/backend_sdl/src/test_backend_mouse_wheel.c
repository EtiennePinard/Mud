#include "../../../mud/include/mud.h"
#include "../../../backends/sdl_backend/include/sdl_backend.h"
#include "test_utils.h"

static int wheelCalled = 0;

Mud_BoxEventResult onWheel(Mud_Event* event, Mud_App* app) {
    (void)app;

    wheelCalled++;

    check(event->mouseWheel.x == 1);
    check(event->mouseWheel.y == -2);

    return MUD_HANDLED_EVENT;
}

int main(void) {
    Mud_App app;

    check(Mud_init(&app, NULL) == MUD_CONTINUE);

    SDL_Event event;

    event.type = SDL_EVENT_MOUSE_WHEEL;
    event.wheel.x = 1;
    event.wheel.y = 2;

    SDL_PumpEvents();
    SDL_FlushEvents(0, -1);
    SDL_PushEvent(&event);

    Mud_LayoutBox box = {
        .isActive = true,
        .renderRect = {0, 0, 10, 10},
        .onMouseWheelScrolled = onWheel,
    };
    app.scene.sceneLayout.numLayoutBox = 1;
    app.scene.sceneLayout.layoutBoxes = &box;

    check(Mud_handleEvents(&app) == MUD_CONTINUE);

    check(wheelCalled == 0);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);

    return 0;
}
