// Defining a macro indicating which backend is used
#ifndef MUD_BACKEND_SDL
#define MUD_BACKEND_SDL

#define MUDSDLBACKEND_VERSION_MAJOR 0
#define MUDSDLBACKEND_VERSION_MINOR 1
#define MUDSDLBACKEND_VERSION_PATCH 0

#define MUDSDLBACKEND_VERSION_STRING                                           \
    (TO_STRING(MUDSDLBACKEND_VERSION_MAJOR) "." TO_STRING(                     \
        MUDSDLBACKEND_VERSION_MINOR) "." TO_STRING(MUDSDLBACKEND_VERSION_PATCH))

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <mud.h>

/**
 * @brief The options used to initialize the SDL backend. For
 * the sdlFlags and windowFlags, see the SDL documentation for
 * SDL_init and SDL_CreateWindowAndRenderer.
 *
 */
typedef struct MudSDLBackend_Options {
    Uint32 sdlFlags; // The initialization flags for the sdl library
    const char* window_title;
    int windowX;
    int windowY;
    int windowWidth;
    int windowHeight;
    Uint32 windowFlags;
} MudSDLBackend_Options;

/**
 * @brief The type of text to be rendered.
 * SINGLE_LINE text renders the entire string on the same line.
 * MULTI_LINE will start another line at each '\n' character
 *
 */
typedef enum TextType { SINGLE_LINE, MULTI_LINE } TextType;

/**
 * @brief Information about the font to use the type of text
 * to render for a Mud_Text primitive.
 *
 */
typedef struct MudSDLBackend_FontData {
    TTF_Font* fontToUse;
    TextType textFitMethod;
} MudSDLBackend_FontData;

/**
 * @brief Opens a font from the font path and populates the data pointers.
 * If data is NULL will allocated data on the heap and return the pointer.
 * Is is the caller's responsibility to free the returned memory.
 *
 * @param data The font data to fill. Can be NULL
 * @param fontPath The path to the font file
 * @param pointSize The point size to open the font in
 * @param textType The type of text to render
 * @return MudSDLBackend_FontData* The data parameter or a heap allocated
 * pointer if data was NULL
 */
MUD_API MudSDLBackend_FontData*
MudSDLBackend_openFontData(MudSDLBackend_FontData* data, const char* fontPath,
                           float pointSize, TextType textType);

/**
 * @brief Frees the data inside the data pointer.
 * Does not free the pointer itself.
 *
 * @param data The pointer to the font data to free
 */
MUD_API void MudSDLBackend_closeFontData(MudSDLBackend_FontData* data);

/**
 * @brief Information about rendering a Mud_TextureQuad
 * primitive. This struct can be initialize from an image file
 * using the MudSDLBackend_createTexture function. The
 * freeAfterRendering parameter controls if the backend should
 * call the SDL_DestroyTexture function wit the texture parameter
 * immediately after rendering. This is useful when your textures are
 * short-lived and are almost guaranteed to change everytime a rerender
 * is triggered.
 *
 */
typedef struct MudSDLBackend_TextureData {
    SDL_Texture* texture;
    bool freeAfterRendering;
} MudSDLBackend_TextureData;

/**
 * @brief Loads a texture from an image file using the SDL_Img library.
 * If data is NULL will allocated data on the heap and return the pointer.
 * Is is the caller's responsibility to free the returned memory.
 *
 * @param data The texture data to fill. Can be NULL
 * @param filename Path to image file
 * @param freeAfterRendering If the texture should be freed after being
 * rendered.
 * @return MudSDLBackend_TextureData* The data parameter or a heap allocated
 * pointer if data was NULL
 */
MUD_API MudSDLBackend_TextureData*
MudSDLBackend_createTexture(MudSDLBackend_TextureData* data,
                            const char* filename, bool freeAfterRendering);

/**
 * @brief Frees the data inside the data pointer.
 * Does not free the pointer itself.
 *
 * @param textureData The texture data to destroy
 */
MUD_API void
MudSDLBackend_destroyTexture(MudSDLBackend_TextureData* textureData);

/**
 * @brief Returns the SDL_Window pointer associated to this application.
 * If you want to get the rendering context associated to this window please use
 * SDL_GetRenderer
 *
 * @return SDL_Window* The SDL_Window pointer associated to this application
 */
MUD_API SDL_Window* MudSDLBackend_getWindow();

/**
 * @brief Returns the Mud SDL backend version string.
 *
 * @return MUD_API const* The Mud SDL backend version string formatted in
 * "major.minor.patch"
 */
MUD_API const char* MudSDLBackend_getVersion();

#ifndef NDEBUG // This code only runs on debug mode
#define DEFAULT_PRIMITIVE_RENDERED_SIZE (16)
typedef struct MudSDLBackend_DebugStats {
    int framesStarted;
    int framesFinished;
    int nb_primitivesRendered;

    Mud_Primitive* primitivesRendered;
} MudSDLBackend_DebugStats;

MUD_API const MudSDLBackend_DebugStats* MudSDLBackend_getDebugStats();
MUD_API void MudSDLBackend_resetDebugStats();

#endif /* NDEBUG */

#endif /* MUD_BACKEND_SDL */
