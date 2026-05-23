#include "sdl_internalState.h"

void MudBackend_terminate() {
    if (state.renderer) {
        SDL_DestroyRenderer(state.renderer);
        state.renderer = NULL;
    }
    if (state.window) {
        SDL_DestroyWindow(state.window);
        state.window = NULL;
    }

    TTF_Quit();
    SDL_Quit();
}
