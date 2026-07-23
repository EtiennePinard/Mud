#ifndef CBB04BEA_DBCB_49C6_A352_DB16D626B1F3
#define CBB04BEA_DBCB_49C6_A352_DB16D626B1F3

#include <mud.h>

#define MudCommon_WHITE (Mud_Color){ 0xFF, 0xFF, 0xFF, 0xFF }
#define MudCommon_BLACK (Mud_Color){ 0x00, 0x00, 0x00, 0xFF }

#define MudCommon_returnOnFailure(mud_appResults)                              \
    do {                                                                       \
        if ((mud_appResults) != MUD_CONTINUE)                                  \
            return MUD_TERMINATE_WITH_SUCCESS;                                 \
    } while (0)

/**
 * @brief Adds a filled quad to the app's render queue.
 *
 * @param app The app to add the primitive to
 * @param renderRect The rectangle the filled quad should be rendered in
 * @param color The color of the filled quad
 * @return Mud_AppResult If the app should continue or terminate with failure
 */
Mud_AppResult MudCommon_addFilledQuad(Mud_App* app, Mud_Rect renderRect,
                                      Mud_Color color);

/**
 * @brief Adds a border quad to the app's render queue.
 *
 * @param app The app to add the primitive to
 * @param renderRect The rectangle the primitive should be rendered in
 * @param color The color of the border quad
 * @param borderThickness The thickness of the border
 * @return Mud_AppResult If the app should continue or terminate with failure
 */
Mud_AppResult MudCommon_addBorderQuad(Mud_App* app, Mud_Rect renderRect,
                                      Mud_Color color, float borderThickness);

/**
 * @brief Adds a text primitive to the app's render queue.
 *
 * @param app The app to add the primitive to
 * @param renderRect The rectangle the primitive should be rendered in
 * @param text The actual text string to be rendered
 * @param color The color of the text
 * @param fontData Backend specific font data
 * @return Mud_AppResult If the app should continue or terminate with failure
 */
Mud_AppResult MudCommon_addText(Mud_App* app, Mud_Rect renderRect,
                                const char* text, Mud_Color color,
                                void* fontData);

/**
 * @brief Adds a textured quad to the app's render queue.
 *
 * @param app The app to add the primitive to
 * @param renderRect The rectangle the primitive should be rendered in
 * @param fontData Backend specific texture data
 * @return Mud_AppResult If the app should continue or terminate with failure
 */
Mud_AppResult MudCommon_addTextureQuad(Mud_App* app, Mud_Rect renderRect,
                                       void* textureData);

/**
 * @brief Scales a Mud_Rect to its parent following a widthRatio and
 * heightRatio. The resulting rect is placed at x = 0 and y = 0. Use
 * MudCommon_centerRectInContainer to center it in the parent rectangle.
 *
 * @param parent The parent rectangle.
 * @param widthRatio The ratio between the resulting rect's width and the
 * parent's width
 * @param heightRatio The ratio between the resulting rect's height and the
 * parent's height
 * @return Mud_Rect The rect scaled in regards to a parent with a width and
 * height ratio placed at (0, 0)
 */
static inline Mud_Rect MudCommon_scaleRectToParent(Mud_Rect parent,
                                                   float widthRatio,
                                                   float heightRatio) {
    return (Mud_Rect){
        .x = 0,
        .y = 0,
        .width = parent.width * widthRatio,
        .height = parent.height * heightRatio,
    };
}

/**
 * @brief Centers a rect in a container rect. For the x and y positions to be
 * non-negative, the rectToCenter should be strictly smaller then the container
 * rect.
 *
 * @param rectToCenter The rect to center in the container
 * @param containerRect The rect containing the rect to center
 * @return Mud_Rect The centered rect
 */
static inline Mud_Rect MudCommon_centerRectInContainer(Mud_Rect rectToCenter,
                                                       Mud_Rect containerRect) {
    return (Mud_Rect){
        .x = (containerRect.width - rectToCenter.width) / 2.0f,
        .y = (containerRect.height - rectToCenter.height) / 2.0f,
        .width = rectToCenter.width,
        .height = rectToCenter.height,
    };
}

/**
 * @brief Request a rerender from the main thread as an event callback.
 *
 * @param event Any event
 * @param app The app to send the main thread rerender request to.
 * @return Mud_AppResult If the operation was successful or not.
 */
Mud_AppResult MudCommon_eventRequestRerender(Mud_Event* event, Mud_App* app);

/**
 * @brief Sets the rerenderLayoutNextFrame field to MUD_MAIN_THREAD_RERENDER.
 * This will tell the Mud_render function to render the layout boxes and tells
 * it that this render request was from the main thread.
 *
 * @param app The app to send the render request to.
 */
static inline void MudCommon_requestRenderFromMainThread(Mud_App* app) {
    atomic_store(&app->rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);
}

/**
 * @brief Sets the rerenderLayoutNextFrame field to MUD_OTHER_THREAD_RERENDER.
 * This will tell the Mud_render function to render the layout boxes and tells
 * it that this render request was from another thread than the main thread.
 *
 * @param app The app to send the render request to.
 */
static inline void MudCommon_requestRenderFromOtherThread(Mud_App* app) {
    atomic_store(&app->rerenderLayoutNextFrame, MUD_OTHER_THREAD_RERENDER);
}

#endif /* CBB04BEA_DBCB_49C6_A352_DB16D626B1F3 */
