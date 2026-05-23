#include <stdlib.h>

#include "../include/mud_backend.h"

Mud_Rect Mud_getWindowRect() { return MudBackend_getWindowRect(); }

Mud_AppResult Mud_addPrimitive(Mud_App* app, Mud_Primitive* primitive) {
    if (app->renderQueue.count >= app->renderQueue.capacity) {
        app->renderQueue.capacity *= 2;
        app->renderQueue.commands =
            realloc(app->renderQueue.commands, app->renderQueue.capacity);
        if (app->renderQueue.commands == NULL)
            return MUD_TERMINATE_WITH_FAILURE;
    }
    app->renderQueue.commands[app->renderQueue.count++] = *primitive;
    return MUD_CONTINUE;
}

Mud_AppResult Mud_render(Mud_App* app) {
    // We only rerender if we are told so
    if (atomic_load(&app->rerenderLayoutNextFrame) == MUD_NO_RERENDER) {
        return MUD_CONTINUE;
    }
    else if (atomic_load(&app->rerenderLayoutNextFrame) ==
             MUD_OTHER_THREAD_RERENDER) {
        // If we already have a MUD_OTHER_THREAD_RERENDER value then we don't
        // want to rerender twice so we set the value to
        // MUD_MAIN_THREAD_RERENDER
        atomic_store(&app->rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);
    }

    // Clearing render queue
    app->renderQueue.count = 0;

    Mud_Layout sceneLayout = app->scene.sceneLayout;
    Mud_AppResult result = MUD_CONTINUE;
    for (size_t index = 0; index < sceneLayout.numLayoutBox; index++) {
        Mud_LayoutBox layoutBox = sceneLayout.layoutBoxes[index];
        if (!layoutBox.isActive) continue;
        Mud_RenderFunction renderFunction = layoutBox.renderFunction;
        if (renderFunction) result = renderFunction(layoutBox.renderRect, app);
        if (result != MUD_CONTINUE) return result;
    }

    result = MudBackend_prepareRender(app->scene.sceneLayout.bgColor);
    if (result != MUD_CONTINUE) return result;

    // Rendering every primitive
    for (size_t index = 0; index < app->renderQueue.count; index++) {
        result = MudBackend_renderPrimitive(&app->renderQueue.commands[index]);
        if (result != MUD_CONTINUE) return result;
    }

    result = MudBackend_finishRender();
    if (result != MUD_CONTINUE) return result;

    // After we rerender, we assume that shouldRender is either OTHER_THREAD or
    // MAIN_THREAD This means that if shouldRender is MUD_NO_RERENDER then it
    // will stay that way else we go to a main thread rerender so that we
    // rerender once more before stopping
    if (atomic_load(&app->rerenderLayoutNextFrame) ==
        MUD_OTHER_THREAD_RERENDER) {
        atomic_store(&app->rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);
    }
    else { atomic_store(&app->rerenderLayoutNextFrame, MUD_NO_RERENDER); }

    return MUD_CONTINUE;
}
