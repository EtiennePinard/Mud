#include "event_helpers.h"
Mud_LayoutBox* Mud_findTopLayoutBox(const Mud_Layout* layout, Mud_Point point) {
    for (size_t i = layout->numLayoutBox; i-- > 0;) {
        Mud_LayoutBox* box = &layout->layoutBoxes[i];

        if (Mud_layoutBoxContainsPoint(box, point)) return box;
    }

    return NULL;
}

Mud_AppResult Mud_dispatchBoxEvent(Mud_App* app, Mud_Event* event,
                                   Mud_Point point,
                                   Mud_EventCallbackGetter getCallback) {
    Mud_Layout* layout = &app->scene.sceneLayout;
    for (size_t i = layout->numLayoutBox; i-- > 0;) {
        Mud_LayoutBox* box = &layout->layoutBoxes[i];

        if (!Mud_layoutBoxContainsPoint(box, point)) continue;

        Mud_BoxEventCallback callback = getCallback(box);

        if (!callback) continue;

        Mud_BoxEventResult result = callback(event, app);

        if (result == MUD_PASS_EVENT) continue;

        switch (result) {
        case MUD_HANDLED_EVENT:
            return MUD_CONTINUE;
        case MUD_TERMINATE_EVENT_SUCCESS:
            return MUD_TERMINATE_WITH_SUCCESS;
        case MUD_TERMINATE_EVENT_FAILURE:
            return MUD_TERMINATE_WITH_FAILURE;
        default:
            __builtin_unreachable();
        }
    }
    return MUD_CONTINUE;
}
