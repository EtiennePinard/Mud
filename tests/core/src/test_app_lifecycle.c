#include "../../../mud/include/mud.h"
#include "../test_backend/include/test_backend.h"

#include "test_utils.h"

static int backendInitCalled = 0;
Mud_AppResult test_backendInit(void* options) {
    (void)options;
    backendInitCalled++;
    return MUD_CONTINUE;
}
static int backendTerminateCalled = 0;
void test_backendTerminate() { backendTerminateCalled++; }
static MudTestBackend_Options backendOptions = { .init = test_backendInit,
                                                 .terminate =
                                                     test_backendTerminate };

#define SCENE_DATA (42)
static int sceneData = SCENE_DATA;
static int terminateSceneCalled = 0;
void test_sceneTerminate(Mud_Scene* scene, Mud_AppResult result) {
    terminateSceneCalled++;
    check(result == MUD_TERMINATE_WITH_SUCCESS);
    check(*((int*)scene->data) == SCENE_DATA);
}

int main(void) {
    Mud_App app;

    check(Mud_init(&app, &backendOptions) == MUD_CONTINUE);
    check(backendInitCalled == 1);

    app.scene.sceneId = 0;
    app.scene.data = &sceneData;
    app.scene.terminateSceneFunction = test_sceneTerminate;

    for (int counter = 0; counter < 10; ++counter) {
        Mud_handleEvents(&app);
        if (Mud_render(&app) != MUD_CONTINUE) break;
    }

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    check(backendTerminateCalled == 1);
    check(terminateSceneCalled == 1);
    return 0;
}
