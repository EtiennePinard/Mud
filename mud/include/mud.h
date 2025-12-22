#ifndef A63348C3_BA40_4628_8469_A854BD996C7F
#define A63348C3_BA40_4628_8469_A854BD996C7F

#define MUD_VERSION_MAJOR 0
#define MUD_VERSION_MINOR 1
#define MUD_VERSION_PATCH 0

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)
#define MUD_VERSION_STRING (TO_STRING(MUD_VERSION_MAJOR) "." TO_STRING(MUD_VERSION_MINOR) "." TO_STRING(MUD_VERSION_PATCH))

#include "mud_types.h"
#include "mud_primitives.h"
#include "mud_render.h"
#include "mud_events.h"
#include "mud_layout.h"
#include "mud_scene.h"
#include "mud_app.h"

struct Mud_App {
    // All data needed for rendering the app
    Mud_RenderQueue renderQueue;
    // To avoid unnecessary renders
    atomic_int rerenderLayoutNextFrame;

    // Layout and event information for RenderBoxes
    Mud_Scene scene;

    // Global events and mouse state
    Mud_AppEvents events;
};

/**
 * @brief Initializes the Mud framework, rendering backend
 * and creates a window to render onto. The user owns the
 * Mud_App* pointer, and is responsible for allocating
 * and freeing that memory.
 *
 * @param app A Mud_App pointer to initialize. Must not be NULL.
 * @param backendOption Options to be passed to the backend. The format
 * of the options are defined by which backend is used. These options
 * can include window options, like the window's name and size.
 * @return Mud_AppResult If the app should continue or terminate with success or failure
 */
MUD_API Mud_AppResult Mud_init(Mud_App* app, void* backendOption);

/**
 * @brief Terminates a Mud_App. This function will call
 * all relevant terminating functions for the app to terminate
 * properly. This function will not free the Mud_App* pointer,
 * it is the caller's responsibility to deal with the memory.
 *
 * @param app A pointer to the Mud_App to terminate
 * @param result If the app terminates with failure or success
 */
MUD_API void Mud_terminate(Mud_App* app, Mud_AppResult result);

/**
 * @brief Returns the mud version string.
 *
 * @return const char* The Mud version string formatted in "major.minor.patch"
 */
MUD_API const char* Mud_getVersion();

#endif /* A63348C3_BA40_4628_8469_A854BD996C7F */
