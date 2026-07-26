/**
 * @file mud_sdl_hello_world.c
 * @author Etienne Pinard
 * @brief This example showcases a typical Mud application lifecycle,
 * which does the following things:
 * 1. Initialize the Mud API, SDL backend, and the scene
 * 2. Uses the mud_widget and mud_common library when rendering and event
 * handling
 * 3. Renders the layout only when needed
 * 4. Terminates
 *
 * @date 2026-07-26
 */

#include <assert.h>
#include <mud_layout.h>
#include <stdio.h>
#include <stdlib.h>

#include <mud.h>
#include <mud_common.h>
#include <mud_widgets.h>
#include <sdl_backend.h>

// Enough for "Hello, World! ()\0" and log_10(2^32)
#define TEXT_MAX_SIZE (30)

typedef struct HelloWorldSceneData {
    u32 counter;
    MudWidget_ButtonStyle buttonstyle;
} HelloWorldSceneData;

static inline void setButtonText(HelloWorldSceneData* sceneData) {
    snprintf(sceneData->buttonstyle.text, TEXT_MAX_SIZE, "Hello, World! (%u)",
             sceneData->counter);
}

// -----------------------------------------------------------------------------
// Event callbacks
// -----------------------------------------------------------------------------

Mud_AppResult incrementCounter(Mud_Event* event, Mud_App* app) {
    // Setting the amount to add base on which mouse button is clicked
    u32 addingToCounter = 1;
    if (event->mouseButton.mouseButtonIndex == MUD_RIGHT_MOUSE_BUTTON) {
        addingToCounter *= 2;
    }
    HelloWorldSceneData* sceneData = (HelloWorldSceneData*)app->scene.data;
    sceneData->counter += addingToCounter;
    setButtonText(sceneData);
    MudCommon_requestRenderFromMainThread(app);
    return MUD_CONTINUE;
}

// -----------------------------------------------------------------------------
// Scene layout
// -----------------------------------------------------------------------------

#define BUTTON_WIDTH_RATIO (0.5)
#define BUTTON_HEIGHT_RATIO (0.5)

Mud_AppResult computeInitialSceneLayout(Mud_App* app) {
    Mud_Rect parentRect = Mud_getWindowRect();

    MudWidget_ButtonStyle helloWorldButton =
        ((HelloWorldSceneData*)app->scene.data)->buttonstyle;
    Mud_LayoutBox buttonLayoutBox = MudWidget_makeButton(
        MudCommon_centerRectInContainer(
            MudCommon_scaleRectToParent(parentRect, BUTTON_WIDTH_RATIO,
                                        BUTTON_HEIGHT_RATIO),
            parentRect),
        &helloWorldButton);
    buttonLayoutBox.onMouseButtonUp = incrementCounter;
    buttonLayoutBox.onMouseEntered = MudCommon_eventRequestRerender;
    buttonLayoutBox.onMouseExited = MudCommon_eventRequestRerender;
    buttonLayoutBox.onMouseButtonDown = MudCommon_eventRequestRerender;
    app->scene.sceneLayout.layoutBoxes[0] = buttonLayoutBox;
    return MUD_CONTINUE;
}

// -----------------------------------------------------------------------------
// Scene creation
// -----------------------------------------------------------------------------

void createSceneData(Mud_App* app) {
    HelloWorldSceneData* sceneData = malloc(sizeof(HelloWorldSceneData));
    assert(sceneData && "Out of memory");
    sceneData->counter = 0;
    app->scene.data = sceneData;
}

#define FONT_PATH ("./assets/font/Edwin-Roman.ttf")
void createWidgets(Mud_App* app) {
    HelloWorldSceneData* sceneData = app->scene.data;
    MudWidget_initButtonStyle(&sceneData->buttonstyle,
                              (Mud_Color){ 0xFF, 0xFF, 0xFF, 0x1F },
                              (Mud_Color){ 0xFF, 0xFF, 0xFF, 0x3F },
                              (Mud_Color){ 0xFF, 0xFF, 0xFF, 0x7F },
                              MudCommon_BLACK, MudCommon_WHITE, 1.0);
    sceneData->buttonstyle.fontData =
        MudSDLBackend_openFontData(NULL, FONT_PATH, 10.0, SINGLE_LINE);
    // Exiting if we could not open the font. The openFontData function will log
    // an error
    if (!sceneData->buttonstyle.fontData) { exit(EXIT_FAILURE); }
    sceneData->buttonstyle.text = malloc(TEXT_MAX_SIZE * sizeof(char));
    assert(sceneData->buttonstyle.text && "Out of memory");
    setButtonText(sceneData);
}
#define NUM_SCENE_LAYOUT_BOXES (1)

void terminateInitialScene(Mud_Scene* scene, Mud_AppResult result) {
    HelloWorldSceneData* sceneData = scene->data;
    MudWidget_destroyButtonStyle(&sceneData->buttonstyle,
                                 (DestroyFontData)MudSDLBackend_closeFontData);
    MudCommon_destroySceneMemory(scene);
    printf("Terminating the scene with result: %s\n",
           result == MUD_TERMINATE_WITH_FAILURE ? "failure" : "success");
}
void createLayout(Mud_App* app) {
    MudCommon_exitOnFailure(
        MudCommon_allocateLayoutBoxes(app, NUM_SCENE_LAYOUT_BOXES));
    app->scene.sceneLayout.bgColor = (Mud_Color){ 0x7f, 0x7f, 0x7f, 0xff };
    app->scene.computeSceneLayout = computeInitialSceneLayout;
    app->scene.terminateSceneFunction = terminateInitialScene;
    app->scene.computeSceneLayout(app);
}

#define INITIAL_SCENE_ID (1)

void initScene(Mud_App* app) {
    app->scene.sceneId = INITIAL_SCENE_ID;

    createSceneData(app);
    createWidgets(app);
    createLayout(app);

    MudCommon_requestRenderFromMainThread(app);
}

// -----------------------------------------------------------------------------
// Application entry point
// -----------------------------------------------------------------------------
MudSDLBackend_Options options = { .window_title = "Mud SDL Hello World",
                                  .windowWidth = 900,
                                  .windowHeight = 600,
                                  .windowX = SDL_WINDOWPOS_CENTERED,
                                  .windowY = SDL_WINDOWPOS_CENTERED };
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
