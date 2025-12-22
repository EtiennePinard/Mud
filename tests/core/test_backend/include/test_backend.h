// Defining a macro indicating which backend is used
#ifndef MUD_BACKEND_TEST
#define MUD_BACKEND_TEST

#include "../../../../mud/include/mud.h"

typedef struct MudTestBackend_Options {
    Mud_AppResult(*init)(void* options);
    void (*terminate)();
    Mud_AppResult(*prepareRender)(Mud_Color clearColor);
    Mud_AppResult(*renderPrimitive)(Mud_Primitive* primitive);
    Mud_AppResult(*finishRender)();
    Mud_AppResult(*popEvent)(Mud_Event* outEvent);
    Mud_Rect(*getWindowRect)();
} MudTestBackend_Options;

#endif /* MUD_BACKEND_TEST */
