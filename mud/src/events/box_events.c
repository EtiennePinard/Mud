#include <assert.h>

#include "box_events.h"
#include "event_helpers.h"

static Mud_BoxEventCallback
getMouseButtonDownCallback(const Mud_LayoutBox* box) {
    return box->onMouseButtonDown;
}
static Mud_BoxEventCallback getMouseButtonUpCallback(const Mud_LayoutBox* box) {
    return box->onMouseButtonUp;
}
static Mud_BoxEventCallback getMouseWheelCallback(const Mud_LayoutBox* box) {
    return box->onMouseWheelScrolled;
}

Mud_AppResult Mud_handleMouseMove(Mud_App* app, Mud_Event* event) {
    assert(event->type == MUD_EVENT_MOUSE_MOVE);
    Mud_LayoutBox* prevTop = Mud_findTopLayoutBox(
        &app->scene.sceneLayout, app->events.mouseState.mousePoint);

    // Update mouse state
    app->events.mouseState.mousePoint =
        (Mud_Point){ .x = event->mouseMove.x, .y = event->mouseMove.y };
    app->events.mouseState.holdingLeftMouseButton =
        event->mouseMove.pressedButtonFlag & MUD_LEFT_BUTTON_MASK;
    app->events.mouseState.holdingRightMouseButton =
        event->mouseMove.pressedButtonFlag & MUD_RIGHT_BUTTON_MASK;

    Mud_LayoutBox* curTop = Mud_findTopLayoutBox(
        &app->scene.sceneLayout, app->events.mouseState.mousePoint);

    if (prevTop != curTop) {
        if (curTop)
            Mud_returnOnFailure(
                Mud_callBoxCallback(curTop->onMouseEntered, event, app));
        if (prevTop)
            Mud_returnOnFailure(
                Mud_callBoxCallback(prevTop->onMouseExited, event, app));
    }
    if (curTop)
        Mud_returnOnFailure(
            Mud_callBoxCallback(curTop->onMouseHovered, event, app));

    return MUD_CONTINUE;
}

static inline void updateMouseButton(Mud_App* app, Mud_Event* event) {
    app->events.mouseState.mousePoint =
        (Mud_Point){ .x = event->mouseButton.x, .y = event->mouseButton.y };
    if (event->mouseButton.mouseButtonIndex == MUD_LEFT_MOUSE_BUTTON) {
        app->events.mouseState.holdingLeftMouseButton =
            event->type == MUD_EVENT_MOUSE_BUTTON_DOWN;
    }
    else if (event->mouseButton.mouseButtonIndex == MUD_RIGHT_MOUSE_BUTTON) {
        app->events.mouseState.holdingRightMouseButton =
            event->type == MUD_EVENT_MOUSE_BUTTON_DOWN;
    }
}
Mud_AppResult Mud_handleMouseButtonDown(Mud_App* app, Mud_Event* event) {
    assert(event->type == MUD_EVENT_MOUSE_BUTTON_DOWN);
    updateMouseButton(app, event);
    return Mud_dispatchBoxEvent(app, event, app->events.mouseState.mousePoint,
                                getMouseButtonDownCallback);
}
Mud_AppResult Mud_handleMouseButtonUp(Mud_App* app, Mud_Event* event) {
    assert(event->type == MUD_EVENT_MOUSE_BUTTON_UP);
    updateMouseButton(app, event);
    return Mud_dispatchBoxEvent(app, event, app->events.mouseState.mousePoint,
                                getMouseButtonUpCallback);
}
Mud_AppResult Mud_handleMouseWheel(Mud_App* app, Mud_Event* event) {
    assert(event->type == MUD_EVENT_MOUSE_WHEEL);
    return Mud_dispatchBoxEvent(app, event, app->events.mouseState.mousePoint,
                                getMouseWheelCallback);
}