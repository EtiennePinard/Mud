#include "global_events.h"
#include "event_helpers.h"
#include <assert.h>

Mud_AppResult Mud_handleQuit(Mud_App* app, Mud_Event* event) {
    assert(event->type == MUD_EVENT_QUIT);
    if (app->events.onQuit) return app->events.onQuit(event, app);
    // The default quit event is to terminate with success
    return MUD_TERMINATE_WITH_SUCCESS;
}
Mud_AppResult Mud_handleKeyDown(Mud_App* app, Mud_Event* event) {
    assert(event->type == MUD_EVENT_KEY_DOWN);
    return Mud_callGlobalCallback(app->events.onKeyDown, event, app);
}
Mud_AppResult Mud_handleKeyUp(Mud_App* app, Mud_Event* event) {
    assert(event->type == MUD_EVENT_KEY_UP);
    return Mud_callGlobalCallback(app->events.onKeyUp, event, app);
}
Mud_AppResult Mud_handleWindowResize(Mud_App* app, Mud_Event* event) {
    assert(event->type == MUD_EVENT_WINDOW_RESIZE);
    return Mud_callGlobalCallback(app->events.onWindowResize, event, app);
}