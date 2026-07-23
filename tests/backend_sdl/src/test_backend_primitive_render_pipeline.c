#include "../../../backends/sdl_backend/include/sdl_backend.h"
#include "../../../mud/include/mud.h"

#include "test_utils.h"

static Mud_AppResult renderTop(Mud_LayoutBox* box, Mud_App* app) {
    Mud_Rect rect = box->renderRect;
    Mud_Primitive p = { .filledQuad = { .type = MUD_PRIMITIVE_FILLED_QUAD,
                                        .renderRect = rect,
                                        .color = { 1, 0, 0, 1 } } };
    return Mud_addPrimitive(app, &p);
}

static Mud_AppResult renderMultiple(Mud_LayoutBox* box, Mud_App* app) {
    Mud_Rect rect = box->renderRect;
    for (int i = 0; i < 5; i++) {
        Mud_Primitive p = { .filledQuad = {
                                .type = MUD_PRIMITIVE_FILLED_QUAD,
                                .renderRect = { rect.x + i * rect.width / 5,
                                                rect.y + i * rect.width / 5,
                                                rect.width / 5, rect.height / 5 },
                                .color = { 0, 1, 0, 1 } } };
        Mud_addPrimitive(app, &p);
    }
    return MUD_CONTINUE;
}

int main(void) {
    Mud_App app;
    check(Mud_init(&app, NULL) == MUD_CONTINUE);
    const MudSDLBackend_DebugStats* stats = MudSDLBackend_getDebugStats();

    Mud_LayoutBox box = { .isActive = true,
                          .renderRect = { 0, 0, 100, 100 },
                          .renderFunction = renderTop };

    app.scene.sceneLayout.layoutBoxes = &box;
    app.scene.sceneLayout.numLayoutBox = 1;

    MudSDLBackend_resetDebugStats();

    atomic_store(&app.rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);
    Mud_render(&app);

    check(stats->nb_primitivesRendered == 1);

    MudSDLBackend_resetDebugStats();
    box.renderFunction = renderMultiple;
    atomic_store(&app.rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);
    Mud_render(&app);
    check(stats->nb_primitivesRendered == 5);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
}
