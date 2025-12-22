#include "../../../mud/include/mud.h"
#include "../test_backend/include/test_backend.h"

#include "test_utils.h"

static Mud_Event queuedEvent;

Mud_AppResult test_popEvent(Mud_Event* outEvent) {
    *outEvent = queuedEvent;
    // Reset the queueEvent to NONE (simulate event consumption)
    queuedEvent.type = MUD_EVENT_NONE;
    return MUD_CONTINUE;
}

static MudTestBackend_Options backendOptions = {
    .popEvent = test_popEvent
};

void test_mouse_move_updates_mouse_point(Mud_App* app) {
    queuedEvent = (Mud_Event){
        .mouseMove = {
            .type = MUD_EVENT_MOUSE_MOVE,
            .pressedButtonFlag = MUD_LEFT_BUTTON_MASK | MUD_RIGHT_BUTTON_MASK,
            .x = 123.0f,
            .y = 456.0f
        }
    };

    check(app->events.mouseState.holdingLeftMouseButton == false);
    check(app->events.mouseState.holdingRightMouseButton == false);
    check(app->events.mouseState.mousePoint.x == 0.0f);
    check(app->events.mouseState.mousePoint.y == 0.0f);

    check(Mud_handleEvents(app) == MUD_CONTINUE);

    check(app->events.mouseState.holdingLeftMouseButton == true);
    check(app->events.mouseState.holdingRightMouseButton == true);
    check(app->events.mouseState.mousePoint.x == 123.0f);
    check(app->events.mouseState.mousePoint.y == 456.0f);

    // Turning the left and right mouse button off
    queuedEvent = (Mud_Event){
        .mouseMove = {
            .type = MUD_EVENT_MOUSE_MOVE,
            .pressedButtonFlag = 0,
            .x = 120.0f,
            .y = 460.0f
        }
    };
    check(Mud_handleEvents(app) == MUD_CONTINUE);

    check(app->events.mouseState.holdingLeftMouseButton == false);
    check(app->events.mouseState.holdingRightMouseButton == false);
    check(app->events.mouseState.mousePoint.x == 120.0f);
    check(app->events.mouseState.mousePoint.y == 460.0f);
}

void test_left_mouse_button_down_and_up(Mud_App* app) {
    // Button down
    queuedEvent = (Mud_Event){
        .mouseButton = {
            .type = MUD_EVENT_MOUSE_BUTTON_DOWN,
            .mouseButtonIndex = MUD_LEFT_MOUSE_BUTTON,
            .clicks = 1,
            .x = 10.0f,
            .y = 20.0f
        }
    };

    check(app->events.mouseState.holdingLeftMouseButton == false);

    check(Mud_handleEvents(app) == MUD_CONTINUE);

    check(app->events.mouseState.holdingLeftMouseButton == true);
    check(app->events.mouseState.mousePoint.x == 10.0f);
    check(app->events.mouseState.mousePoint.y == 20.0f);

    // Button up
    queuedEvent = (Mud_Event){
        .mouseButton = {
            .type = MUD_EVENT_MOUSE_BUTTON_UP,
            .mouseButtonIndex = MUD_LEFT_MOUSE_BUTTON,
            .clicks = 1,
            .x = 10.0f,
            .y = 20.0f
        }
    };

    check(Mud_handleEvents(app) == MUD_CONTINUE);

    check(app->events.mouseState.holdingLeftMouseButton == false);
    check(app->events.mouseState.mousePoint.x == 10.0f);
    check(app->events.mouseState.mousePoint.y == 20.0f);
}

void test_right_mouse_button_down_and_up(Mud_App* app) {
    // Button down
    queuedEvent = (Mud_Event){
        .mouseButton = {
            .type = MUD_EVENT_MOUSE_BUTTON_DOWN,
            .mouseButtonIndex = MUD_RIGHT_MOUSE_BUTTON,
            .clicks = 1,
            .x = 30.0f,
            .y = 40.0f
        }
    };

    check(app->events.mouseState.holdingRightMouseButton == false);

    check(Mud_handleEvents(app) == MUD_CONTINUE);

    check(app->events.mouseState.holdingRightMouseButton == true);
    check(app->events.mouseState.mousePoint.x == 30.0f);
    check(app->events.mouseState.mousePoint.y == 40.0f);

    // Button up
    queuedEvent = (Mud_Event){
        .mouseButton = {
            .type = MUD_EVENT_MOUSE_BUTTON_UP,
            .mouseButtonIndex = MUD_RIGHT_MOUSE_BUTTON,
            .clicks = 1,
            .x = 30.0f,
            .y = 40.0f
        }
    };

    check(Mud_handleEvents(app) == MUD_CONTINUE);

    check(app->events.mouseState.holdingRightMouseButton == false);
    check(app->events.mouseState.mousePoint.x == 30.0f);
    check(app->events.mouseState.mousePoint.y == 40.0f);
}

int main(void) {
    Mud_App app;
    check(Mud_init(&app, &backendOptions) == MUD_CONTINUE);

    test_mouse_move_updates_mouse_point(&app);
    test_left_mouse_button_down_and_up(&app);
    test_right_mouse_button_down_and_up(&app);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    return 0;
}
