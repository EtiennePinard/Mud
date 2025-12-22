#ifndef FA551E0E_4D9F_4936_9CB1_C61ADB1746AE
#define FA551E0E_4D9F_4936_9CB1_C61ADB1746AE

#include <stddef.h>

#include "mud_types.h"
#include "mud_app.h"
#include "mud_primitives.h"

typedef struct Mud_RenderQueue {
    Mud_Primitive* commands;
    size_t count;
    size_t capacity;
} Mud_RenderQueue;

/**
 * @brief Adds a primitive to the render queue of the app.
 * 
 * @param app The app to a primitive to its render queue
 * @param primitive The primitive to add
 * @return Mud_AppResult MUD_CONTINUE on success or MUD_TERMINATE_WITH_FAILURE on failure
 */
Mud_AppResult Mud_addPrimitive(Mud_App* app, Mud_Primitive* primitive);

typedef enum Mud_ReRenderValue {
    MUD_NO_RERENDER,
    MUD_MAIN_THREAD_RERENDER,
    MUD_OTHER_THREAD_RERENDER
} Mud_ReRenderValue;

/**
 * @brief This function gets called every frame right after all
 * events in the app were handled.
 *
 * @param app The state and events of the app
 * @return If the app should continue or terminate with failure or success
 */
MUD_API Mud_AppResult Mud_render(Mud_App* app);


#endif /* FA551E0E_4D9F_4936_9CB1_C61ADB1746AE */
