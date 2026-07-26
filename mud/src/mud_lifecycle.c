#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/mud_backend.h"

#define INITIAL_RENDER_QUEUE_SIZE (16)

static bool hasInitOnce = false;
static bool hasTerminatedOnce = false;

Mud_AppResult Mud_init(Mud_App* app, void* backendOption) {
    if (app == NULL) {
        fprintf(stderr, "MUD ERROR: app pointer is NULL. Exiting program\n");
        exit(EXIT_FAILURE);
    }

    if (hasInitOnce) return MUD_CONTINUE;
    hasInitOnce = true;

    if (hasTerminatedOnce) {
        // We have terminated once but we call Mud_init again
        // This means that we will need to terminate again, so we set
        // hasTerminatedOnce to false.
        hasTerminatedOnce = false;
    }

    // We memset to a know valid state
    memset(app, 0, sizeof(Mud_App));

    // Initialize commands
    app->renderQueue.capacity = INITIAL_RENDER_QUEUE_SIZE;
    app->renderQueue.commands =
        malloc(app->renderQueue.capacity * sizeof(Mud_Primitive));
    if (app->renderQueue.commands == NULL) {
        // We are missing memory
        return MUD_TERMINATE_WITH_FAILURE;
    }

    // Initialize backend
    return MudBackend_init(backendOption);
}

void Mud_terminate(Mud_App* app, Mud_AppResult result) {
    if (hasTerminatedOnce || !hasInitOnce) return;
    hasTerminatedOnce = true;

    if (hasInitOnce) {
        // We have initializes once and we called Mud_terminate.
        // If we want to call Mud_init again, we need to set
        // hasInitOnce to false.
        hasInitOnce = false;
    }

    if (app->scene.terminateSceneFunction) {
        app->scene.terminateSceneFunction(&app->scene, result);
    }

    // Cleaning up after ourselves
    if (app->renderQueue.commands) {
        free(app->renderQueue.commands);
        app->renderQueue.commands = NULL;
    }
    // Make sure the backend also cleans up itself
    MudBackend_terminate();
}

const char* Mud_getVersion(void) { return MUD_VERSION_STRING; }