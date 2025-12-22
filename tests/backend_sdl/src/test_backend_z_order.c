#include "../../../mud/include/mud.h"
#include "../../../backends/sdl_backend/include/sdl_backend.h"

#include "test_utils.h"

#define RENDER_RECT { 0, 0, 100, 100 }

Mud_Primitive topQuad = {
    .filledQuad = {
        .type = MUD_PRIMITIVE_FILLED_QUAD,
        .renderRect = RENDER_RECT,
        .color = {1, 0, 0, 1}
    }
};
Mud_Primitive bottomQuad = {
    .borderQuad = {
        .type = MUD_PRIMITIVE_BORDER_QUAD,
        .renderRect = RENDER_RECT,
        .color = {1, 0, 0, 1},
        .borderThickness = 2.0f
    }
};

static Mud_AppResult renderTop(Mud_Rect rect, Mud_App* app) { 
    (void) rect;
    return Mud_addPrimitive(app, &topQuad);
}

static Mud_AppResult renderBottom(Mud_Rect rect, Mud_App* app) {
    (void) rect;
    return Mud_addPrimitive(app, &bottomQuad);
}

static Mud_LayoutBox top = {
    .isActive = true,
    .renderRect = RENDER_RECT,
    .renderFunction = renderTop
};
static Mud_LayoutBox bottom = {
    .isActive = true,
    .renderRect = RENDER_RECT,
    .renderFunction = renderBottom
};

int main(void) {
    Mud_App app;
    check(Mud_init(&app, NULL) == MUD_CONTINUE);
    const MudSDLBackend_DebugStats* stats = MudSDLBackend_getDebugStats();

    Mud_LayoutBox layoutBoxes[2] = { bottom, top };

    app.scene.sceneLayout.layoutBoxes = layoutBoxes;
    app.scene.sceneLayout.numLayoutBox = 2;

    MudSDLBackend_resetDebugStats();

    atomic_store(&app.rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);
    Mud_render(&app);

    check(stats->framesStarted == 1);
    check(stats->nb_primitivesRendered == 2);
    check(stats->framesFinished == 1);
    check(memcmp(&stats->primitivesRendered[0], &bottomQuad, sizeof(Mud_FilledQuad)) == 0);
    check(memcmp(&stats->primitivesRendered[1], &topQuad, sizeof(Mud_BorderQuad)) == 0);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
}
