#ifndef C12A7874_D9F0_4009_A4D8_9AC217A477D0
#define C12A7874_D9F0_4009_A4D8_9AC217A477D0

#include "../include/mud.h"

/**
 * @brief This function is called at the start of the program
 * to initialize the backend.
 *
 * @param options A pointer to options to the backend. The format of
 * the option data are defined by each backend. This pointer can be NULL.
 * @return Mud_AppResult If the app should continue or terminate with failure or success
 */
MUD_API extern Mud_AppResult MudBackend_init(void* options);

/**
 * @brief This function is called when the app terminates
 * to terminate and cleanup the backend.
 *
 */
MUD_API extern void MudBackend_terminate();

/**
 * @brief This function is called right before Mud starts
 * rendering the layout. The backend should ready itself for rendering
 * and clear the screen with the clear color.
 *
 * @param clearColor The color to clear the screen with
 * @return Mud_AppResult If the app should continue or terminate with failure or success
 */
MUD_API extern Mud_AppResult MudBackend_prepareRender(Mud_Color clearColor);

/**
 * @brief Renders a Mud primitive to the screen.
 *
 * @param primitive The primitive to render
 * @return Mud_AppResult If the app should continue or terminate with failure or success
 */
MUD_API extern Mud_AppResult MudBackend_renderPrimitive(Mud_Primitive* primitive);

/**
 * @brief Finishes the rendering process. This function is called
 * after all primitives are done rendering.
 *
 * @param clearColor
 * @return Mud_AppResult If the app should continue or terminate with failure or success
 */
MUD_API extern Mud_AppResult MudBackend_finishRender();

/**
 * @brief Pops an event from the render queue formed by the Mud backend. If
 * there is no event to process, the backend does not modify the outEvent
 * parameter.
 *
 * @param outEvent A pointer to place the popped event into
 * @return Mud_AppResult If the app should continue or terminate with failure or success
 */
MUD_API extern Mud_AppResult MudBackend_popEvent(Mud_Event* outEvent);

/**
 * @brief Returns the rectangle defining the window. The width and height of 
 * the rect is the window size, and this rectangle is positioned at (0, 0).
 * 
 * @return Mud_Rect The window's rectangle. 
 */
MUD_API extern Mud_Rect MudBackend_getWindowRect();

#endif /* C12A7874_D9F0_4009_A4D8_9AC217A477D0 */
