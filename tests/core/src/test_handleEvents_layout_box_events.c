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

static MudTestBackend_Options backendOptions = { .popEvent = test_popEvent };

static int mouseDownCalled = 0;
static int mouseUpCalled = 0;
static int mouseEnteredCalled = 0;
static int mouseHoveredCalled = 0;
static int mouseExitedCalled = 0;
static int mouseWheelCalled = 0;

void resetCounters() {
    mouseDownCalled = 0;
    mouseUpCalled = 0;
    mouseEnteredCalled = 0;
    mouseHoveredCalled = 0;
    mouseExitedCalled = 0;
    mouseWheelCalled = 0;
}

Mud_BoxEventResult onMouseDown(Mud_Event* e, Mud_App* app) {
    (void)e;
    (void)app;
    mouseDownCalled++;
    return MUD_HANDLED_EVENT;
}

Mud_BoxEventResult onMouseUp(Mud_Event* e, Mud_App* app) {
    (void)e;
    (void)app;
    mouseUpCalled++;
    return MUD_HANDLED_EVENT;
}

Mud_BoxEventResult onMouseUpPassEvent(Mud_Event* e, Mud_App* app) {
    (void)e;
    (void)app;
    mouseUpCalled++;
    return MUD_PASS_EVENT;
}

Mud_BoxEventResult onMouseEntered(Mud_Event* e, Mud_App* app) {
    (void)e;
    (void)app;
    mouseEnteredCalled++;
    return MUD_HANDLED_EVENT;
}

Mud_BoxEventResult onMouseHovered(Mud_Event* e, Mud_App* app) {
    (void)e;
    (void)app;
    mouseHoveredCalled++;
    return MUD_HANDLED_EVENT;
}

Mud_BoxEventResult onMouseExited(Mud_Event* e, Mud_App* app) {
    (void)e;
    (void)app;
    mouseExitedCalled++;
    return MUD_HANDLED_EVENT;
}

Mud_BoxEventResult onMouseWheel(Mud_Event* e, Mud_App* app) {
    (void)e;
    (void)app;
    mouseWheelCalled++;
    return MUD_HANDLED_EVENT;
}

void setupSingleLayoutBox(Mud_App* app) {
    static Mud_LayoutBox box;

    box.isActive = true;
    box.renderRect = (Mud_Rect){ 10, 10, 100, 100 };

    box.onMouseButtonDown = onMouseDown;
    box.onMouseButtonUp = onMouseUp;
    box.onMouseEntered = onMouseEntered;
    box.onMouseHovered = onMouseHovered;
    box.onMouseExited = onMouseExited;
    box.onMouseWheelScrolled = onMouseWheel;

    app->scene.sceneLayout.layoutBoxes = &box;
    app->scene.sceneLayout.numLayoutBox = 1;
}
static Mud_LayoutBox boxes[2];

void setupTwoLayoutBoxesTopNoCallbacks(Mud_App* app) {
    boxes[0].isActive = true;
    boxes[0].renderRect = (Mud_Rect){ 10, 10, 100, 100 };
    boxes[0].onMouseEntered = onMouseEntered;
    boxes[0].onMouseHovered = onMouseHovered;
    boxes[0].onMouseExited = onMouseExited;
    boxes[0].onMouseButtonDown = onMouseDown;
    boxes[0].onMouseButtonUp = onMouseUp;
    boxes[0].onMouseWheelScrolled = onMouseWheel;

    boxes[1].isActive = true;
    boxes[1].renderRect = (Mud_Rect){ 10, 10, 200, 200 };

    app->scene.sceneLayout.layoutBoxes = boxes;
    app->scene.sceneLayout.numLayoutBox = 2;
}

void setupTwoLayoutBoxesTopPassEvent(Mud_App* app) {
    boxes[0].isActive = true;
    boxes[0].renderRect = (Mud_Rect){ 10, 10, 100, 100 };
    boxes[0].onMouseEntered = onMouseEntered;
    boxes[0].onMouseHovered = onMouseHovered;
    boxes[0].onMouseExited = onMouseExited;
    boxes[0].onMouseButtonDown = onMouseDown;
    boxes[0].onMouseButtonUp = onMouseUp;
    boxes[0].onMouseWheelScrolled = onMouseWheel;

    boxes[1].isActive = true;
    boxes[1].renderRect = (Mud_Rect){ 10, 10, 200, 200 };
    boxes[1].onMouseButtonUp = onMouseUpPassEvent;

    app->scene.sceneLayout.layoutBoxes = boxes;
    app->scene.sceneLayout.numLayoutBox = 2;
}

void test_mouse_outside_triggers_no_callbacks(Mud_App* app) {
    resetCounters();

    queuedEvent = (Mud_Event){
        .mouseMove = { .type = MUD_EVENT_MOUSE_MOVE, .x = 0, .y = 0 }
    };

    Mud_handleEvents(app);

    check(mouseEnteredCalled == 0);
    check(mouseHoveredCalled == 0);
}

void test_mouse_click_inside_box(Mud_App* app) {
    resetCounters();

    queuedEvent =
        (Mud_Event){ .mouseButton = { .type = MUD_EVENT_MOUSE_BUTTON_DOWN,
                                      .mouseButtonIndex = MUD_LEFT_MOUSE_BUTTON,
                                      .x = 50,
                                      .y = 50 } };
    Mud_handleEvents(app);

    queuedEvent =
        (Mud_Event){ .mouseButton = { .type = MUD_EVENT_MOUSE_BUTTON_UP,
                                      .mouseButtonIndex = MUD_LEFT_MOUSE_BUTTON,
                                      .x = 50,
                                      .y = 50 } };
    Mud_handleEvents(app);

    check(mouseDownCalled == 1);
    check(mouseUpCalled == 1);
}

void test_mouse_enter_box(Mud_App* app) {
    resetCounters();

    queuedEvent = (Mud_Event){
        .mouseMove = { .type = MUD_EVENT_MOUSE_MOVE, .x = 0, .y = 0 }
    };
    Mud_handleEvents(app);
    check(mouseEnteredCalled == 0);
    check(mouseHoveredCalled == 0);

    queuedEvent = (Mud_Event){
        .mouseMove = { .type = MUD_EVENT_MOUSE_MOVE, .x = 20, .y = 20 }
    };
    Mud_handleEvents(app);

    check(mouseEnteredCalled == 1);
    check(mouseHoveredCalled == 1);
}

void test_mouse_hover_box(Mud_App* app) {
    resetCounters();

    // Reset the mouse to be outside the layout box
    queuedEvent = (Mud_Event){
        .mouseMove = { .type = MUD_EVENT_MOUSE_MOVE, .x = 0, .y = 0 }
    };
    Mud_handleEvents(app);

    queuedEvent = (Mud_Event){
        .mouseMove = { .type = MUD_EVENT_MOUSE_MOVE, .x = 20, .y = 20 }
    };
    Mud_handleEvents(app);

    queuedEvent = (Mud_Event){
        .mouseMove = { .type = MUD_EVENT_MOUSE_MOVE, .x = 30, .y = 30 }
    };
    Mud_handleEvents(app);

    queuedEvent = (Mud_Event){
        .mouseMove = { .type = MUD_EVENT_MOUSE_MOVE, .x = 40, .y = 40 }
    };
    Mud_handleEvents(app);

    check(mouseEnteredCalled == 1);
    check(mouseHoveredCalled == 3);
}

void test_mouse_exit_box(Mud_App* app) {
    resetCounters();

    queuedEvent = (Mud_Event){
        .mouseMove = { .type = MUD_EVENT_MOUSE_MOVE, .x = 20, .y = 20 }
    };
    Mud_handleEvents(app);

    queuedEvent = (Mud_Event){
        .mouseMove = { .type = MUD_EVENT_MOUSE_MOVE, .x = 200, .y = 200 }
    };
    Mud_handleEvents(app);

    check(mouseExitedCalled == 1);
}

void test_mouse_wheel_hovering_box(Mud_App* app) {
    resetCounters();

    queuedEvent = (Mud_Event){
        .mouseMove = { .type = MUD_EVENT_MOUSE_MOVE, .x = 20, .y = 20 }
    };
    Mud_handleEvents(app);

    queuedEvent = (Mud_Event){
        .mouseWheel = { .type = MUD_EVENT_MOUSE_WHEEL, .x = 0, .y = 1 }
    };
    Mud_handleEvents(app);

    check(mouseWheelCalled == 1);
}

void test_mouse_move_does_not_skip_top_box_without_callbacks(Mud_App* app) {
    resetCounters();

    queuedEvent = (Mud_Event){
        .mouseMove = { .type = MUD_EVENT_MOUSE_MOVE, .x = 20, .y = 20 }
    };

    Mud_handleEvents(app);

    check(mouseEnteredCalled == 0);
    check(mouseHoveredCalled == 0);
    check(mouseExitedCalled == 0);
}

void test_mouse_click_skips_top_box_with_no_callbacks(Mud_App* app) {
    resetCounters();

    queuedEvent =
        (Mud_Event){ .mouseButton = { .type = MUD_EVENT_MOUSE_BUTTON_DOWN,
                                      .mouseButtonIndex = MUD_LEFT_MOUSE_BUTTON,
                                      .x = 20,
                                      .y = 20 } };

    Mud_handleEvents(app);

    check(mouseDownCalled == 1);
}

void test_mouse_click_pass_event_behaviour(Mud_App* app) {
    resetCounters();

    queuedEvent =
        (Mud_Event){ .mouseButton = { .type = MUD_EVENT_MOUSE_BUTTON_UP,
                                      .mouseButtonIndex = MUD_LEFT_MOUSE_BUTTON,
                                      .x = 20,
                                      .y = 20 } };

    Mud_handleEvents(app);

    check(mouseUpCalled == 2);
}

int main(void) {
    Mud_App app;
    check(Mud_init(&app, &backendOptions) == MUD_CONTINUE);

    setupSingleLayoutBox(&app);

    test_mouse_outside_triggers_no_callbacks(&app);
    test_mouse_click_inside_box(&app);
    test_mouse_enter_box(&app);
    test_mouse_hover_box(&app);
    test_mouse_exit_box(&app);
    test_mouse_wheel_hovering_box(&app);

    setupTwoLayoutBoxesTopNoCallbacks(&app);

    test_mouse_move_does_not_skip_top_box_without_callbacks(&app);
    test_mouse_click_skips_top_box_with_no_callbacks(&app);

    setupTwoLayoutBoxesTopPassEvent(&app);
    test_mouse_click_pass_event_behaviour(&app);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    return 0;
}
