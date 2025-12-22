#include "../../../mud/include/mud.h"
#include "../../../backends/sdl_backend/include/sdl_backend.h"
#include "test_utils.h"


static int mouseButtonDownCalled = 0;
static int mouseButtonUpCalled = 0;

Mud_AppResult onMouseButtonDown(Mud_Event* event, Mud_App* app) {
    (void)app;

    check(event->mouseButton.mouseButtonIndex == MUD_RIGHT_MOUSE_BUTTON);
    mouseButtonDownCalled++;

    return MUD_CONTINUE;
}

Mud_AppResult onMouseButtonUp(Mud_Event* event, Mud_App* app) {
    (void)event;
    (void)app;

    check(event->mouseButton.mouseButtonIndex == MUD_RIGHT_MOUSE_BUTTON);
    mouseButtonUpCalled++;

    return MUD_CONTINUE;
}

int main(void) {
    Mud_App app;

    check(Mud_init(&app, NULL) == MUD_CONTINUE);

    SDL_Event event;

    event.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
    event.button.button = SDL_BUTTON_RIGHT;
    event.button.x = 50.0f;
    event.button.y = 60.0f;

    Mud_LayoutBox box = {
        .isActive = true,
        .renderRect = {0, 0, 100, 100},
        .onMouseButtonDown = onMouseButtonDown,
        .onMouseButtonUp = onMouseButtonUp
    };
    app.scene.sceneLayout.numLayoutBox = 1;
    app.scene.sceneLayout.layoutBoxes = &box;

    SDL_PumpEvents();
    SDL_FlushEvents(0, -1);
    SDL_PushEvent(&event);

    check(Mud_handleEvents(&app) == MUD_CONTINUE);

    check(app.events.mouseState.holdingRightMouseButton == true);
    check(mouseButtonDownCalled == 1);
    check(app.events.mouseState.mousePoint.x == 50.0f);
    check(app.events.mouseState.mousePoint.y == 60.0f);

    event.type = SDL_EVENT_MOUSE_BUTTON_UP;
    event.button.button = SDL_BUTTON_RIGHT;
    event.button.x = 20.0f;
    event.button.y = 30.0f;


    SDL_PumpEvents();
    SDL_FlushEvents(0, -1);
    SDL_PushEvent(&event);

    check(Mud_handleEvents(&app) == MUD_CONTINUE);

    check(app.events.mouseState.holdingRightMouseButton == false);
    check(mouseButtonDownCalled == 1);
    check(app.events.mouseState.mousePoint.x == 20.0f);
    check(app.events.mouseState.mousePoint.y == 30.0f);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);

    return 0;
}
