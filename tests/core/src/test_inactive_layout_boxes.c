#include "../../../mud/include/mud.h"
#include "../test_backend/include/test_backend.h"

#include "test_utils.h"

static Mud_Event queuedEvent;

Mud_AppResult test_popEvent(Mud_Event* outEvent) {
    *outEvent = queuedEvent;
    return MUD_CONTINUE;
}

static MudTestBackend_Options backendOptions = {
    .popEvent = test_popEvent
};

static int topBoxClicked = 0;
static int bottomBoxClicked = 0;
Mud_AppResult onTopBoxClick(Mud_Event* e, Mud_App* app) {
    (void)e; (void)app;
    topBoxClicked++;
    return MUD_CONTINUE;
}
Mud_AppResult onBottomBoxClick(Mud_Event* e, Mud_App* app) {
    (void)e; (void)app;
    bottomBoxClicked++;
    return MUD_CONTINUE;
}

static int topBoxRendered = 0;
static int bottomBoxRendered = 0;
Mud_AppResult bottomBoxRender(Mud_Rect rect, Mud_App* app) {
    (void)rect; (void)app;
    bottomBoxRendered++;
    return MUD_CONTINUE;
}
Mud_AppResult topBoxRender(Mud_Rect rect, Mud_App* app) {
    (void)rect; (void)app;
    topBoxRendered++;
    return MUD_CONTINUE;
}

static void resetCounters(void) {
    topBoxClicked = 0;
    bottomBoxClicked = 0;
    topBoxRendered = 0;
    bottomBoxRendered = 0;
}

void test_inactive_box_receives_no_events(Mud_App* app) {
    resetCounters();

    Mud_LayoutBox box = {
        .isActive = false,
        .renderRect = { 10, 10, 50, 50 },
        .renderFunction = topBoxRender,
        .onMouseButtonDown = onTopBoxClick
    };

    app->scene.sceneLayout.layoutBoxes = &box;
    app->scene.sceneLayout.numLayoutBox = 1;

    queuedEvent = (Mud_Event){
        .mouseButton = {
            .type = MUD_EVENT_MOUSE_BUTTON_DOWN,
            .x = 20,
            .y = 20,
            .mouseButtonIndex = MUD_LEFT_MOUSE_BUTTON
        }
    };
    atomic_store(&app->rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);

    Mud_handleEvents(app);
    Mud_render(app);

    check(topBoxClicked == 0);
    check(topBoxRendered == 0);
}

void test_inactive_box_does_not_block_active_box(Mud_App* app) {
    resetCounters();

    Mud_LayoutBox bottomBox = {
        .isActive = true,
        .renderRect = { 10, 10, 50, 50 },
        .renderFunction = bottomBoxRender,
        .onMouseButtonDown = onBottomBoxClick
    };

    Mud_LayoutBox topBox = {
        .isActive = false,
        .renderRect = { 10, 10, 50, 50 },
        .renderFunction = topBoxRender,
        .onMouseButtonDown = onTopBoxClick
    };

    Mud_LayoutBox layoutBoxes[2] = { bottomBox, topBox };
    app->scene.sceneLayout.layoutBoxes = layoutBoxes;
    app->scene.sceneLayout.numLayoutBox = 2;

    queuedEvent = (Mud_Event){
        .mouseButton = {
            .type = MUD_EVENT_MOUSE_BUTTON_DOWN,
            .x = 20,
            .y = 20,
            .mouseButtonIndex = MUD_LEFT_MOUSE_BUTTON
        }
    };
    atomic_store(&app->rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);

    Mud_handleEvents(app);
    Mud_render(app);

    check(topBoxClicked == 0);
    check(topBoxRendered == 0);
    check(bottomBoxClicked == 1);
    check(bottomBoxRendered == 1);
}

int main(void) {
    Mud_App app;
    check(Mud_init(&app, &backendOptions) == MUD_CONTINUE);

    test_inactive_box_receives_no_events(&app);
    test_inactive_box_does_not_block_active_box(&app);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    return 0;
}
