#ifndef C3A93085_0A46_4010_AB8E_474494B9589C
#define C3A93085_0A46_4010_AB8E_474494B9589C

#include "../include/sdl_backend.h"

typedef struct MudSDLBackend_State {
    MudSDLBackend_Options options;
    SDL_Window* window;
    SDL_Renderer* renderer;
} MudSDLBackend_State;

extern MudSDLBackend_State state;

/**
 * @brief Renders single line text centered and fitted inside a rectangle.
 * 
 * @param font The font to render the text with. 
 * @param text The text to render
 * @param color The color of the text
 * @param rectToFit The rectangle to fit the text into
 * @return Mud_AppResult If the app should continue or terminate with failure or success
 */
Mud_AppResult renderSingleLineTextCenteredToFit(TTF_Font* font, const char* text, SDL_Color color, SDL_FRect rectToFit);

/**
 * @brief Renders multiline text center justified and fitted inside a rectangle.
 * 
 * @param font The font to render the text with. 
 * @param text The text to render
 * @param color The color of the text
 * @param rectToFit The rectangle to fit the text into
 * @return Mud_AppResult If the app should continue or terminate with failure or success
 */
Mud_AppResult renderMultilineTextCenteredToFit(TTF_Font* font, const char* text, SDL_Color color, SDL_FRect rectToFit);

#define TODO SDL_assert(false && "TODO: Implement");

#ifndef NDEBUG 
extern MudSDLBackend_DebugStats debugStats;
#endif /* NDEBUG */

#endif /* C3A93085_0A46_4010_AB8E_474494B9589C */
