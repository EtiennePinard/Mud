#include <SDL3_image/SDL_image.h>

#include "sdl_internalState.h"

MudSDLBackend_TextureData* MudSDLBackend_createTexture(MudSDLBackend_TextureData* data, const char* filename, bool freeAfterRendering) {
    SDL_Texture* texture = IMG_LoadTexture(state.renderer, filename);

    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create texture from file %s with error %s\n", filename, SDL_GetError());
        return NULL;
    }

    if (!data) data = SDL_malloc(sizeof(MudSDLBackend_TextureData));
    if (!data) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Out of memory, Buy more RAM LOL");
        return NULL;
    }

    data->texture = texture;
    data->freeAfterRendering = freeAfterRendering;

    return data;
}

void MudSDLBackend_destroyTexture(MudSDLBackend_TextureData* textureData) {
    if (!textureData) return;

    SDL_DestroyTexture(textureData->texture);
}
