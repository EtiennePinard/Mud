#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <mud.h>
#include <mud_common.h>
#include <sdl_backend.h>

// Enough for "Hello, World! ()\0" and log_10(2^32)
#define TEXT_MAX_SIZE (30)

typedef struct InitialSceneData {
    MudSDLBackend_FontData fontData;
    char helloWorldText[TEXT_MAX_SIZE];
    u32 counter;
    Mud_Color filledColor;
    Mud_Color borderColor;
    Mud_Color textColor;
} InitialSceneData;

Mud_AppResult renderHelloWorldButton(Mud_Rect rect, Mud_App* app) {
    InitialSceneData* sceneData = (InitialSceneData*)app->scene.data;
    MudCommon_returnOnFailure(
        MudCommon_addFilledQuad(app, rect, sceneData->filledColor));
    MudCommon_returnOnFailure(
        MudCommon_addBorderQuad(app, rect, sceneData->borderColor, 1.0));
    return MudCommon_addText(app, rect, sceneData->helloWorldText,
                             sceneData->textColor, &sceneData->fontData);
}

Mud_AppResult incrementCounter(Mud_Event* event, Mud_App* app) {
    // Setting the amount to add base on which mouse button is clicked
    u32 addingToCounter = 1;
    if (event->mouseButton.mouseButtonIndex == MUD_RIGHT_MOUSE_BUTTON) {
        addingToCounter *= 2;
    }
    // Updating the counter and the text in the scene data
    InitialSceneData* sceneData = (InitialSceneData*)app->scene.data;
    sceneData->counter += addingToCounter;
    snprintf(sceneData->helloWorldText, TEXT_MAX_SIZE, "Hello, World! (%u)",
             sceneData->counter);
    MudCommon_requestRenderFromMainThread(app);
    return MUD_CONTINUE;
}

Mud_AppResult highlightButton(Mud_Event* event, Mud_App* app) {
    (void)event;
    ((InitialSceneData*)app->scene.data)->filledColor.a = 0x3f;
    MudCommon_requestRenderFromMainThread(app);
    return MUD_CONTINUE;
}

Mud_AppResult unHighlightButton(Mud_Event* event, Mud_App* app) {
    (void)event;
    ((InitialSceneData*)app->scene.data)->filledColor.a = 0x1f;
    MudCommon_requestRenderFromMainThread(app);
    return MUD_CONTINUE;
}

#define BUTTON_WIDTH_RATIO (0.5)
#define BUTTON_HEIGHT_RATIO (0.5)

Mud_AppResult computeInitialSceneLayout(Mud_App* app) {
    Mud_Rect parentRect = Mud_getWindowRect();

    app->scene.sceneLayout.layoutBoxes[0] = (Mud_LayoutBox){
        .isActive = true,
        .renderRect = MudCommon_centerRectInContainer(
            MudCommon_scaleRectToParent(parentRect, BUTTON_WIDTH_RATIO,
                                        BUTTON_HEIGHT_RATIO),
            parentRect),
        .renderFunction = renderHelloWorldButton,
        .onMouseEntered = highlightButton,
        .onMouseExited = unHighlightButton,
        .onMouseButtonUp = incrementCounter
    };

    return MUD_CONTINUE;
}

void terminateInitialScene(void* data, Mud_AppResult result) {
    (void)result;
    InitialSceneData* sceneData = (InitialSceneData*)data;
    MudSDLBackend_closeFontData(&sceneData->fontData);
    printf("Terminating the scene with result: %s\n",
           result == MUD_TERMINATE_WITH_FAILURE ? "failure" : "success");
}

#define INITIAL_SCENE_ID (1)
#define FONT_PATH ("./assets/font/Edwin-Roman.ttf")
#define NUM_SCENE_LAYOUT_BOXES (1)

void initScene(Mud_App* app) {
    app->scene.sceneId = INITIAL_SCENE_ID;

    InitialSceneData* sceneData = malloc(sizeof(InitialSceneData));
    assert(sceneData && "Buy more RAM lol");
    MudSDLBackend_openFontData(&sceneData->fontData, FONT_PATH, 10.0,
                               SINGLE_LINE);
    sceneData->counter = 0;
    snprintf(sceneData->helloWorldText, TEXT_MAX_SIZE, "Hello, World! (%u)",
             sceneData->counter);
    sceneData->filledColor = MudCommon_WHITE;
    sceneData->filledColor.a = 0x1f; // We want light shade of white
    sceneData->borderColor = MudCommon_WHITE;
    sceneData->textColor = MudCommon_BLACK;
    app->scene.data = sceneData;

    app->scene.sceneLayout.numLayoutBox = NUM_SCENE_LAYOUT_BOXES;
    app->scene.sceneLayout.layoutBoxes =
        malloc(app->scene.sceneLayout.numLayoutBox * sizeof(Mud_LayoutBox));

    app->scene.sceneLayout.bgColor = (Mud_Color){ 0x7f, 0x7f, 0x7f, 0xff };

    app->scene.computeSceneLayout = computeInitialSceneLayout;
    app->scene.computeSceneLayout(app);
    app->scene.terminateSceneFunction = terminateInitialScene;

    MudCommon_requestRenderFromMainThread(app);
}

MudSDLBackend_Options options = { .window_title = "SDL Hello World",
                                  .windowWidth = 900,
                                  .windowHeight = 600,
                                  .windowX = SDL_WINDOWPOS_CENTERED,
                                  .windowY = SDL_WINDOWPOS_CENTERED };

/**
 * @brief This example showcases a typical Mud application lifecycle,
 * which does the following things:
 * 1. Initialize the Mud API, backend, and the scene
 * 2. Handles events one by one
 * 3. Renders the layout when needed
 * 4. Terminates
 *
 */
int main(void) {
    Mud_App app;
    if (Mud_init(&app, &options) != MUD_CONTINUE) return EXIT_FAILURE;
    initScene(&app);

    Mud_AppResult result;
    while (1) {
        if ((result = Mud_handleEvents(&app)) != MUD_CONTINUE) break;
        if ((result = Mud_render(&app)) != MUD_CONTINUE) break;
    }

    Mud_terminate(&app, result);
    return EXIT_SUCCESS;
}
