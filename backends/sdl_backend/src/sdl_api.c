#include "sdl_internalState.h"

SDL_Window* MudSDLBackend_getWindow() {
    return state.window;
}

Mud_Rect MudBackend_getWindowRect() {
    int width, height;
    SDL_GetWindowSize(state.window, &width, &height);
    return (Mud_Rect) {
        .width = (float)width,
            .height = (float)height,
            .x = 0,
            .y = 0,
    };
}

const char* MudSDLBackend_getVersion() {
    return MUDSDLBACKEND_VERSION_STRING;
}

#ifndef NDEBUG // This code only runs on debug mode
const MudSDLBackend_DebugStats* MudSDLBackend_getDebugStats() { return &debugStats; }

void MudSDLBackend_resetDebugStats() {
    SDL_free(debugStats.primitivesRendered);
    memset(&debugStats, 0, sizeof(MudSDLBackend_DebugStats));
    debugStats.primitivesRendered = SDL_malloc(DEFAULT_PRIMITIVE_RENDERED_SIZE * sizeof(Mud_Primitive));
}
#endif /* NDEBUG */
