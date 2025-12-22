#include "../../../mud/include/mud.h"
#include "../../../backends/sdl_backend/include/sdl_backend.h"
#include "test_utils.h"


static int hoveredCalled = 0;

Mud_AppResult onHover(Mud_Event* event, Mud_App* app) {
    (void)event;
    (void)app;

    hoveredCalled++;

    return MUD_CONTINUE;
}

int main(void) {
    Mud_App app;

    check(Mud_init(&app, NULL) == MUD_CONTINUE);

    SDL_Event event;

    event.type = SDL_EVENT_MOUSE_MOTION;
    event.motion.x = 120.0f;
    event.motion.y = 240.0f;

    Mud_LayoutBox box = {
        .isActive = true,
        .renderRect = {100, 200, 100, 100},
        .onMouseHovered = onHover
    };
    app.scene.sceneLayout.numLayoutBox = 1;
    app.scene.sceneLayout.layoutBoxes = &box;

    SDL_PumpEvents();
    SDL_FlushEvents(0, -1);
    SDL_PushEvent(&event);

    check(Mud_handleEvents(&app) == MUD_CONTINUE);

    check(hoveredCalled == 1);
    check(app.events.mouseState.mousePoint.x == 120.0f);
    check(app.events.mouseState.mousePoint.y == 240.0f);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);

    return 0;
}
