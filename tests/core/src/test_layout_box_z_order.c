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

static int bottomClicked = 0;
static int topClicked = 0;
static int bottomHovered = 0;
static int topHovered = 0;
Mud_BoxEventResult onBottomClick(Mud_Event* e, Mud_App* app) {
    (void)e; (void)app;
    bottomClicked++;
    return MUD_HANDLED_EVENT;
}
Mud_BoxEventResult onTopClick(Mud_Event* e, Mud_App* app) {
    (void)e; (void)app;
    topClicked++;
    return MUD_HANDLED_EVENT;
}
Mud_BoxEventResult onBottomHover(Mud_Event* e, Mud_App* app) {
    (void)e; (void)app;
    bottomHovered++;
    return MUD_HANDLED_EVENT;
}
Mud_BoxEventResult onTopHover(Mud_Event* e, Mud_App* app) {
    (void)e; (void)app;
    topHovered++;
    return MUD_HANDLED_EVENT;
}

static int bottomRendered = 0;
static int topRendered = 0;
Mud_AppResult renderBottom(Mud_LayoutBox* box, Mud_App* app) {
    (void)box, (void)app;
    check(bottomRendered == topRendered);
    bottomRendered++;
    return MUD_CONTINUE;
}
Mud_AppResult renderTop(Mud_LayoutBox* box, Mud_App* app) {
    (void)box, (void)app;
    check(bottomRendered == topRendered + 1);
    topRendered++;
    return MUD_CONTINUE;
}

static void resetCounters(void) {
    bottomClicked = topClicked = 0;
    bottomHovered = topHovered = 0;
    bottomRendered = topRendered = 0;
}

void test_topmost_box_receives_click(Mud_App* app) {
    resetCounters();

    Mud_LayoutBox bottom = {
        .isActive = true,
        .renderRect = { 0, 0, 100, 100 },
        .renderFunction = renderBottom,
        .onMouseButtonDown = onBottomClick
    };

    Mud_LayoutBox top = {
        .isActive = true,
        .renderRect = { 0, 0, 100, 100 },
        .renderFunction = renderTop,
        .onMouseButtonDown = onTopClick
    };

    Mud_LayoutBox layoutBoxes[2] = { bottom, top };
    app->scene.sceneLayout.layoutBoxes = layoutBoxes;
    app->scene.sceneLayout.numLayoutBox = 2;

    queuedEvent = (Mud_Event){
        .mouseButton = {
            .type = MUD_EVENT_MOUSE_BUTTON_DOWN,
            .x = 50,
            .y = 50,
            .mouseButtonIndex = MUD_LEFT_MOUSE_BUTTON
        }
    };
    atomic_store(&app->rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);

    Mud_handleEvents(app);
    Mud_render(app);

    check(topClicked == 1);
    check(bottomClicked == 0);
    check(bottomRendered == 1);
    check(topRendered == 1);
}

void test_hover_respects_z_order(Mud_App* app) {
    resetCounters();

    Mud_LayoutBox bottom = {
        .isActive = true,
        .renderRect = { 0, 0, 100, 100 },
        .renderFunction = renderBottom,
        .onMouseHovered = onBottomHover
    };

    Mud_LayoutBox top = {
        .isActive = true,
        .renderRect = { 0, 0, 100, 100 },
        .renderFunction = renderTop,
        .onMouseHovered = onTopHover
    };

    Mud_LayoutBox layoutBoxes[2] = { bottom, top };
    app->scene.sceneLayout.layoutBoxes = layoutBoxes;
    app->scene.sceneLayout.numLayoutBox = 2;

    queuedEvent = (Mud_Event){
        .mouseMove = {
            .type = MUD_EVENT_MOUSE_MOVE,
            .x = 50,
            .y = 50
        }
    };
    atomic_store(&app->rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);

    Mud_handleEvents(app);
    Mud_render(app);

    check(topHovered == 1);
    check(bottomHovered == 0);
    check(bottomRendered == 1);
    check(topRendered == 1);
}


int main(void) {
    Mud_App app;
    check(Mud_init(&app, &backendOptions) == MUD_CONTINUE);

    test_topmost_box_receives_click(&app);
    test_hover_respects_z_order(&app);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    return 0;
}
