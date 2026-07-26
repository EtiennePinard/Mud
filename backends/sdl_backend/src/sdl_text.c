#define _POSIX_C_SOURCE 200809L
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "sdl_internalState.h"

MudSDLBackend_FontData* MudSDLBackend_openFontData(MudSDLBackend_FontData* data,
                                                   const char* fontPath,
                                                   float pointSize,
                                                   TextType textType) {
    TTF_Font* font = TTF_OpenFont(fontPath, pointSize);

    if (!font) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     "Failed to open font at %s with error %s\n", fontPath,
                     SDL_GetError());
        return NULL;
    }

    if (!data) data = SDL_malloc(sizeof(MudSDLBackend_TextureData));
    if (!data) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Out of memory, Buy more RAM LOL");
        return NULL;
    }

    data->fontToUse = font;
    data->textFitMethod = textType;

    return data;
}

void MudSDLBackend_closeFontData(MudSDLBackend_FontData* data) {
    if (!data) return;

    TTF_CloseFont(data->fontToUse);
}

Mud_AppResult findFontSizeToFit(const char* textString, SDL_FRect rectToFit,
                                TTF_Font* baseFont, bool isTextMultiLine,
                                float* resultingFontSize) {
    Mud_AppResult result = MUD_CONTINUE;
    if (!baseFont || !textString) return result;

    TTF_Font* tempFont = TTF_CopyFont(baseFont);
    if (!tempFont) return result;

    // If we have an empty string simply return the copied font
    if (textString[0] == '\0') {
        *resultingFontSize = 0.0;
        result = MUD_CONTINUE;
        goto end;
    }

    // Initial guess font size
    const float referenceSize = 16.0;

    // Set base font to reference size
    if (!TTF_SetFontSize(tempFont, referenceSize)) goto end;

    int textW = 0, textH = 0;
    bool sizeSuccess = false;
    if (isTextMultiLine)
        sizeSuccess = TTF_GetStringSizeWrapped(tempFont, textString, 0, 0,
                                               &textW, &textH);
    else
        sizeSuccess =
            TTF_GetStringSize(tempFont, textString, 0, &textW, &textH);

    if (!sizeSuccess || textW == 0 || textH == 0) goto end;

    // Compute scale factor based on rectToFit size
    float scaleW = (float)rectToFit.w / (float)textW;
    float scaleH = (float)rectToFit.h / (float)textH;
    float scale = fminf(scaleW, scaleH);

    float finalSize = referenceSize * scale;
    finalSize =
        SDL_clamp(finalSize, 4, rectToFit.h); // Prevent tiny or huge font sizes

    *resultingFontSize = finalSize;
    result = MUD_CONTINUE;
end:
    TTF_CloseFont(tempFont);
    return result;
}

Mud_AppResult renderSingleLineTextCenteredToFit(TTF_Font* font,
                                                const char* text,
                                                SDL_Color color,
                                                SDL_FRect rect) {
    TTF_Font* tempFont = TTF_CopyFont(font);
    if (!tempFont) return MUD_TERMINATE_WITH_FAILURE;

    float fontSizeToFit;
    if (findFontSizeToFit(text, rect, tempFont, false, &fontSizeToFit) !=
        MUD_CONTINUE) {
        TTF_CloseFont(tempFont);
        return MUD_TERMINATE_WITH_FAILURE;
    }
    if (!TTF_SetFontSize(tempFont, fontSizeToFit)) {
        TTF_CloseFont(tempFont);
        return MUD_TERMINATE_WITH_FAILURE;
    }

    SDL_Surface* textSurface =
        TTF_RenderText_Blended_Wrapped(tempFont, text, 0, color, 0);
    if (!textSurface) {
        TTF_CloseFont(tempFont);
        return MUD_TERMINATE_WITH_FAILURE;
    }

    SDL_Texture* textTexture =
        SDL_CreateTextureFromSurface(state.renderer, textSurface);
    if (!textTexture) {
        SDL_DestroySurface(textSurface);
        TTF_CloseFont(tempFont);
        return MUD_TERMINATE_WITH_FAILURE;
    }

    // Centering the text from the baseline
    int ascent = TTF_GetFontAscent(tempFont);
    SDL_FRect renderTextRect = { .x = rect.x + (rect.w - textSurface->w) / 2,
                                 .y = rect.y + (rect.h - ascent) / 2,
                                 .w = textSurface->w,
                                 .h = textSurface->h };
    if (!SDL_RenderTexture(state.renderer, textTexture, NULL, &renderTextRect))
        return MUD_TERMINATE_WITH_FAILURE;
    TTF_CloseFont(tempFont);
    SDL_DestroySurface(textSurface);
    SDL_DestroyTexture(textTexture);
    return MUD_CONTINUE;
}

Mud_AppResult renderMultilineTextCenteredToFit(TTF_Font* font, const char* text,
                                               SDL_Color color,
                                               SDL_FRect rect) {
    if (!font || !text) return MUD_TERMINATE_WITH_FAILURE;

    TTF_Font* tempFont = TTF_CopyFont(font);
    if (!tempFont) return MUD_TERMINATE_WITH_FAILURE;

    float fontSizeToFit;
    if (findFontSizeToFit(text, rect, tempFont, true, &fontSizeToFit) !=
        MUD_CONTINUE) {
        TTF_CloseFont(tempFont);
        return MUD_TERMINATE_WITH_FAILURE;
    }
    if (!TTF_SetFontSize(tempFont, fontSizeToFit)) {
        TTF_CloseFont(tempFont);
        return MUD_TERMINATE_WITH_FAILURE;
    }

    char* textCopy = strdup(text);
    if (!textCopy) {
        TTF_CloseFont(tempFont);
        return MUD_TERMINATE_WITH_FAILURE;
    }

    int lineHeight = TTF_GetFontLineSkip(tempFont);
    int numLines = 0;

    // Count lines
    for (const char* p = text; *p; p++) {
        if (*p == '\n') numLines++;
    }
    numLines++; // at least one line

    int totalTextHeight = numLines * lineHeight;
    int y = rect.y + (rect.h - totalTextHeight) / 2;

    // Tokenize and render each line
    char* saveptr = NULL;
    char* line = strtok_r(textCopy, "\n", &saveptr);
    while (line) {
        SDL_Surface* surf = TTF_RenderText_Blended(tempFont, line, 0, color);
        if (!surf) {
            TTF_CloseFont(tempFont);
            free(textCopy);
            return MUD_TERMINATE_WITH_FAILURE;
        }

        SDL_Texture* tex = SDL_CreateTextureFromSurface(state.renderer, surf);
        if (!tex) {
            TTF_CloseFont(tempFont);
            SDL_DestroySurface(surf);
            free(textCopy);
            return MUD_TERMINATE_WITH_FAILURE;
        }

        SDL_FRect dst = { .x = rect.x + (rect.w - surf->w) / 2,
                          .y = y,
                          .w = surf->w,
                          .h = surf->h };

        SDL_RenderTexture(state.renderer, tex, NULL, &dst);

        y += lineHeight;

        SDL_DestroySurface(surf);
        SDL_DestroyTexture(tex);
        line = strtok_r(NULL, "\n", &saveptr);
    }

    TTF_CloseFont(tempFont);
    free(textCopy);
    return MUD_CONTINUE;
}
