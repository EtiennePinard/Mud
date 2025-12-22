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

static int keyDownCalled = 0;
Mud_AppResult onKeyDown(Mud_Event* event, Mud_App* app) {
    (void)app;
    keyDownCalled++;
    check(event->type == MUD_EVENT_KEY_DOWN);
    check(event->key.scancode == 42);
    check(event->key.keycode == 97);
    return MUD_CONTINUE;
}

static int keyUpCalled = 0;
Mud_AppResult onKeyUp(Mud_Event* event, Mud_App* app) {
    (void)app;
    keyUpCalled++;
    check(event->type == MUD_EVENT_KEY_UP);
    check(event->key.scancode == 42);
    check(event->key.keycode == 97);
    return MUD_CONTINUE;
}

static int windowResizeCalled = 0;
Mud_AppResult onWindowResize(Mud_Event* event, Mud_App* app) {
    (void)app;
    windowResizeCalled++;
    check(event->type == MUD_EVENT_WINDOW_RESIZE);
    check(event->windowResize.width == 42);
    check(event->windowResize.height == 97);
    return MUD_CONTINUE;
}

static int quitCalled = 0;
Mud_AppResult onQuit(Mud_Event* event, Mud_App* app) {
    (void)app;
    quitCalled++;
    check(event->type == MUD_EVENT_QUIT);
    return MUD_CONTINUE;
}

void test_single_keydown_event_is_dispatched(Mud_App* app) {
    queuedEvent = (Mud_Event){
        .key = {
            .type = MUD_EVENT_KEY_DOWN,
            .scancode = 42,
            .keycode = 97
        }
    };
    check(keyDownCalled == 0);
    check(Mud_handleEvents(app) == MUD_CONTINUE);
    check(keyDownCalled == 1);
    check(Mud_handleEvents(app) == MUD_CONTINUE);
    check(keyDownCalled == 1);
}

void test_single_keyup_event_is_dispatched(Mud_App* app) {
    queuedEvent = (Mud_Event){
        .key = {
            .type = MUD_EVENT_KEY_UP,
            .scancode = 42,
            .keycode = 97
        }
    };
    check(keyUpCalled == 0);
    check(Mud_handleEvents(app) == MUD_CONTINUE);
    check(keyUpCalled == 1);
    check(Mud_handleEvents(app) == MUD_CONTINUE);
    check(keyUpCalled == 1);
}

void test_single_windowResize_event_is_dispatched(Mud_App* app) {
    queuedEvent = (Mud_Event){
        .windowResize = {
            .type = MUD_EVENT_WINDOW_RESIZE,
            .width = 42,
            .height = 97
        }
    };
    check(windowResizeCalled == 0);
    check(Mud_handleEvents(app) == MUD_CONTINUE);
    check(windowResizeCalled == 1);
    check(Mud_handleEvents(app) == MUD_CONTINUE);
    check(windowResizeCalled == 1);
}

void test_single_quit_event_is_dispatched(Mud_App* app) {
    queuedEvent = (Mud_Event){
        .quit = {
            .type = MUD_EVENT_QUIT,
        }
    };
    check(quitCalled == 0);
    check(Mud_handleEvents(app) == MUD_CONTINUE);
    check(quitCalled == 1);
    check(Mud_handleEvents(app) == MUD_CONTINUE);
    check(quitCalled == 1);
}

Mud_AppResult test_popEvent_fail(Mud_Event* e) {
    (void)e;
    return MUD_TERMINATE_WITH_FAILURE;
}
void test_backend_failure_terminates_app(Mud_App* app) {
    backendOptions.popEvent = test_popEvent_fail;

    check(Mud_handleEvents(app) == MUD_TERMINATE_WITH_FAILURE);
}

int main(void) {
    Mud_App app;
    check(Mud_init(&app, &backendOptions) == MUD_CONTINUE);
    app.events.onKeyDown = onKeyDown;
    app.events.onKeyUp = onKeyUp;
    app.events.onWindowResize = onWindowResize;
    app.events.onQuit = onQuit;
    test_single_keydown_event_is_dispatched(&app);
    test_single_keyup_event_is_dispatched(&app);
    test_single_windowResize_event_is_dispatched(&app);
    test_single_quit_event_is_dispatched(&app);
    test_backend_failure_terminates_app(&app);
    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    return 0;
}
