#include "sdl_internalState.h"

#ifndef NDEBUG
#define DEBUG_STAT_RENDERED_PRIMITIVE                                          \
  debugStats.primitivesRendered[debugStats.nb_primitivesRendered++] = *primitive
#define DEBUG_STAT_FRAME_STARTED debugStats.framesStarted += 1
#define DEBUG_STAT_FRAMES_FINISHED debugStats.framesFinished += 1
#else
#define DEBUG_STAT_RENDERED_PRIMITIVE ((void)0)
#define DEBUG_STAT_FRAME_STARTED ((void)0)
#define DEBUG_STAT_FRAMES_FINISHED ((void)0)
#endif

#define setRenderDrawColorHelper(color)                                        \
  SDL_SetRenderDrawColor(state.renderer, color.r, color.g, color.b, color.a);

Mud_AppResult MudBackend_prepareRender(Mud_Color clearColor) {
  DEBUG_STAT_FRAME_STARTED;
  setRenderDrawColorHelper(clearColor);
  SDL_RenderClear(state.renderer);
  return MUD_CONTINUE;
}

// To pun cast from Mud_Rect to SDL_FRect
typedef union FRectMudRectPunning {
  SDL_FRect frect;
  Mud_Rect mudRect;
} FRectMudRectPunning;

// To pun cast from Mud_Color to SDL_Color
typedef union SDLColorMudColorPunning {
  Mud_Color mudColor;
  SDL_Color sdlColor;
} SDLColorMudColorPunning;

void drawRectWithThickness(SDL_FRect *outer, float thickness) {
  // Draw 4 filled rectangles for top, bottom, left, right
  SDL_RenderFillRect(state.renderer,
                     &(SDL_FRect){outer->x, outer->y, outer->w, thickness});
  SDL_RenderFillRect(state.renderer,
                     &(SDL_FRect){outer->x, outer->y + outer->h - thickness,
                                  outer->w, thickness});
  SDL_RenderFillRect(state.renderer,
                     &(SDL_FRect){outer->x, outer->y + thickness, thickness,
                                  outer->h - 2 * thickness});
  SDL_RenderFillRect(state.renderer,
                     &(SDL_FRect){outer->x + outer->w - thickness,
                                  outer->y + thickness, thickness,
                                  outer->h - 2 * thickness});
}

Mud_AppResult MudBackend_renderPrimitive(Mud_Primitive *primitive) {
  FRectMudRectPunning rectPun;
  SDLColorMudColorPunning colorPun;
  SDL_BlendMode previousBlendMode;
  MudSDLBackend_TextureData *textureData;
  switch (primitive->type) {
  case MUD_PRIMITIVE_BORDER_QUAD:
    SDL_GetRenderDrawBlendMode(state.renderer, &previousBlendMode);
    SDL_SetRenderDrawBlendMode(state.renderer, SDL_BLENDMODE_BLEND);
    setRenderDrawColorHelper(primitive->borderQuad.color);
    rectPun.mudRect = primitive->borderQuad.renderRect;
    drawRectWithThickness(&rectPun.frect,
                          primitive->borderQuad.borderThickness);
    SDL_SetRenderDrawBlendMode(state.renderer, previousBlendMode);
    DEBUG_STAT_RENDERED_PRIMITIVE;
    break;
  case MUD_PRIMITIVE_FILLED_QUAD:
    SDL_GetRenderDrawBlendMode(state.renderer, &previousBlendMode);
    SDL_SetRenderDrawBlendMode(state.renderer, SDL_BLENDMODE_BLEND);
    setRenderDrawColorHelper(primitive->filledQuad.color);
    rectPun.mudRect = primitive->filledQuad.renderRect;
    SDL_RenderFillRect(state.renderer, &rectPun.frect);
    SDL_SetRenderDrawBlendMode(state.renderer, previousBlendMode);
    DEBUG_STAT_RENDERED_PRIMITIVE;
    break;
  case MUD_PRIMITIVE_TEXTURED_QUAD:
    textureData =
        (MudSDLBackend_TextureData *)primitive->textureQuad.textureData;
    rectPun.mudRect = primitive->textureQuad.renderRect;
    SDL_RenderTexture(state.renderer, textureData->texture, NULL,
                      &rectPun.frect);
    if (textureData->freeAfterRendering) {
      SDL_DestroyTexture(textureData->texture);
    }
    DEBUG_STAT_RENDERED_PRIMITIVE;
    break;
  case MUD_PRIMITIVE_TEXT:
    rectPun.mudRect = primitive->text.renderRect;
    colorPun.mudColor = primitive->text.color;
    switch (
        ((MudSDLBackend_FontData *)primitive->text.fontData)->textFitMethod) {
    case SINGLE_LINE:
      renderSingleLineTextCenteredToFit(
          ((MudSDLBackend_FontData *)primitive->text.fontData)->fontToUse,
          primitive->text.text, colorPun.sdlColor, rectPun.frect);
      break;
    case MULTI_LINE:
      renderMultilineTextCenteredToFit(
          ((MudSDLBackend_FontData *)primitive->text.fontData)->fontToUse,
          primitive->text.text, colorPun.sdlColor, rectPun.frect);
      break;
    default:
      break;
    }
    DEBUG_STAT_RENDERED_PRIMITIVE;
    break;
  default:
    break;
  }

  return MUD_CONTINUE;
}

Mud_AppResult MudBackend_finishRender() {
  if (!SDL_RenderPresent(state.renderer))
    return MUD_TERMINATE_WITH_FAILURE;
  DEBUG_STAT_FRAMES_FINISHED;
  return MUD_CONTINUE;
}
