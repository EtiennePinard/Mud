#include "sdl_internalState.h"

MudSDLBackend_State state = { 0 };
#ifndef NDEBUG
MudSDLBackend_DebugStats debugStats = { 0 };
#endif /* NDEBUG */

#define check_error(condition, function_name)                                  \
    if (condition) {                                                           \
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, function_name " Error: %s\n",     \
                     SDL_GetError());                                          \
        return MUD_TERMINATE_WITH_FAILURE;                                     \
    }

Mud_AppResult MudBackend_init(void* backend_options) {
    // If we have already initialize the renderer and window, continue
    if (state.renderer && state.window) return MUD_CONTINUE;
    if (backend_options != NULL) {
        state = *(MudSDLBackend_State*)backend_options;
    }

    check_error(!SDL_Init(state.options.sdlFlags), "SDL_Init");
    check_error(!TTF_Init(), "TTF_Init");

    check_error(!SDL_CreateWindowAndRenderer(
                    state.options.window_title, state.options.windowWidth,
                    state.options.windowHeight, state.options.windowFlags,
                    &state.window, &state.renderer),
                "SDL_CreateWindowAndRenderer");
    check_error(!SDL_SetWindowPosition(state.window, state.options.windowX,
                                       state.options.windowY),
                "SDL_SetWindowPosition");

    return MUD_CONTINUE;
}
