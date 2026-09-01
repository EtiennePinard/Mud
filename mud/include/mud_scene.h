#ifndef DDA40159_E2A5_4E9F_BDEC_D81CA46D19B3
#define DDA40159_E2A5_4E9F_BDEC_D81CA46D19B3

#include "mud_layout.h"
#include "mud_types.h"

typedef u32 Mud_SceneId;

/**
 * @brief A struct representing a scene in Mud, which is the
 * container of an app's layout.
 *
 */
typedef struct Mud_Scene Mud_Scene;

/**
 * @brief This function is called when it is the first time rendering a scene
 * or when the window size changes. This function is responsible for populating
 * the sceneLayout array.
 *
 */
typedef Mud_AppResult (*ComputeSceneLayout)(Mud_App* app);

/**
 * @brief This is function will free the scene's data.
 * This is function is called by the framework when the
 * app exits. If the scene switches, it is the responsibility
 * of the user of the API to call this function.
 *
 */
typedef void (*TerminateScene)(Mud_Scene* scene, Mud_AppResult result);

struct Mud_Scene {
    Mud_SceneId sceneId;
    Mud_Layout sceneLayout;

    ComputeSceneLayout computeSceneLayout;
    TerminateScene terminateSceneFunction;
    void* data;
};

#endif /* DDA40159_E2A5_4E9F_BDEC_D81CA46D19B3 */
