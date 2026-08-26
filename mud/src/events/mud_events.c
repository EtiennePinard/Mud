#include "../../include/mud_backend.h"
#include "global_events.h"
#include "box_events.h"

Mud_AppResult Mud_handleEvents(Mud_App* app) {
    Mud_Event event;
    event.type = MUD_EVENT_NONE;
    if (MudBackend_popEvent(&event) != MUD_CONTINUE) {
        // Backend error, terminating the app
        return MUD_TERMINATE_WITH_FAILURE;
    }

    switch (event.type) {
    case MUD_EVENT_NONE:
        // If there no event to handle so return
        return MUD_CONTINUE;
    case MUD_EVENT_QUIT:
        return Mud_handleQuit(app, &event);
    case MUD_EVENT_KEY_DOWN:
        return Mud_handleKeyDown(app, &event);
    case MUD_EVENT_KEY_UP:
        return Mud_handleKeyUp(app, &event);
    case MUD_EVENT_WINDOW_RESIZE:
        return Mud_handleWindowResize(app, &event);
    case MUD_EVENT_MOUSE_MOVE:
        return Mud_handleMouseMove(app, &event);
    case MUD_EVENT_MOUSE_BUTTON_DOWN:
        return Mud_handleMouseButtonDown(app, &event);
    case MUD_EVENT_MOUSE_BUTTON_UP:
        return Mud_handleMouseButtonUp(app, &event);
    case MUD_EVENT_MOUSE_WHEEL:
        return Mud_handleMouseWheel(app, &event);
    default:
        break;
    }

    return MUD_CONTINUE;
}
