#ifndef UUID_6237F5B4_165E_4E36_9B48_F3AB14950499_H
#define UUID_6237F5B4_165E_4E36_9B48_F3AB14950499_H

#include "../../include/mud.h"

#define Mud_returnOnFailure(mud_appResults)                                    \
    do {                                                                       \
        if ((mud_appResults) != MUD_CONTINUE) return mud_appResults;           \
    } while (0)

static inline Mud_AppResult Mud_callGlobalCallback(Mud_GlobalEventCallback callback,
                                             Mud_Event* event, Mud_App* app) {
    if (!callback) return MUD_CONTINUE;

    return callback(event, app);
}
static inline Mud_AppResult Mud_callBoxCallback(Mud_BoxEventCallback callback,
                                                Mud_Event* event,
                                                Mud_App* app) {
    if (!callback) return MUD_CONTINUE;
    Mud_BoxEventResult result = callback(event, app);
    switch (result) {
    case MUD_HANDLED_EVENT:
    case MUD_PASS_EVENT:
        return MUD_CONTINUE;
    case MUD_TERMINATE_EVENT_SUCCESS:
        return MUD_TERMINATE_WITH_SUCCESS;
    case MUD_TERMINATE_EVENT_FAILURE:
        return MUD_TERMINATE_WITH_FAILURE;
    }
    __builtin_unreachable();
}

static inline bool Mud_layoutBoxContainsPoint(const Mud_LayoutBox* box,
                                              Mud_Point point) {
    return box->isActive && Mud_pointInRect(point, box->renderRect);
}

/**
 * @brief Finds the topmost Mud_LayoutBox at a particular point.
 * The layout boxes are reversed iterated and are passed to the
 * Mud_layoutBoxContainsPoint function to see if they contain the
 * point. If no layout box is found, NULL is returned.
 *
 * @param layout A pointer to the Mud_Layout of the Mud_App.
 * @param point The point to check if a layout box contains it.
 * @return Mud_LayoutBox* The topmost layout box containing the point,
 * else NULL.
 */
Mud_LayoutBox* Mud_findTopLayoutBox(const Mud_Layout* layout, Mud_Point point);

typedef Mud_BoxEventCallback (*Mud_EventCallbackGetter)(
    const Mud_LayoutBox* box);


Mud_AppResult Mud_dispatchBoxEvent(Mud_App* app, Mud_Event* event,
                                   Mud_Point point,
                                   Mud_EventCallbackGetter getCallback);

#endif /* UUID_6237F5B4_165E_4E36_9B48_F3AB14950499_H */
