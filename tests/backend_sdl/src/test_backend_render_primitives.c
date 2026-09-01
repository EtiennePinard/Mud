#include "../../../mud/include/mud.h"
#include "../../../backends/sdl_backend/include/sdl_backend.h"

#include "test_utils.h"

static Mud_Primitive filledQuad = {
    .filledQuad = {
        .type = MUD_PRIMITIVE_FILLED_QUAD,
        .renderRect = {1, 20, 30, 67},
        .color = {1, 241, 56, 1}
    }
};
static Mud_Primitive borderQuad = {
    .borderQuad = {
        .type = MUD_PRIMITIVE_BORDER_QUAD,
        .renderRect = {15, 200, 45, 21},
        .color = {145, 2, 156, 34},
        .borderThickness = 3.14159
    }
};
static Mud_Primitive text = {
    .text = {
        .type = MUD_PRIMITIVE_TEXT,
        .renderRect = {125.6, 45.7, 123.4, 52.3},
        .color = {34, 56, 78, 90},
        .text = "Test text\nused\nfor testing",
    }
};
static Mud_Primitive textureQuad = {
    .textureQuad = {
        .type = MUD_PRIMITIVE_TEXTURED_QUAD,
        .renderRect = {56.7, 32.5, 163.7, 235235.7}
    }
};

static Mud_AppResult renderFilledQuad(Mud_LayoutBox* box, Mud_App* app) {
    (void)box;
    return Mud_addPrimitive(app, &filledQuad);
}
static Mud_AppResult renderBorderQuad(Mud_LayoutBox* box, Mud_App* app) {
    (void)box;
    return Mud_addPrimitive(app, &borderQuad);
}
static Mud_AppResult renderText(Mud_LayoutBox* box, Mud_App* app) {
    (void)box;
    return Mud_addPrimitive(app, &text);
}
static Mud_AppResult renderTextureQuad(Mud_LayoutBox* box, Mud_App* app) {
    (void)box;
    return Mud_addPrimitive(app, &textureQuad);
}

#define FONT_PATH ("assets/font/Edwin-Roman.ttf")
#define IMG_PATH ("assets/img/black_queen.png")

int main(void) {
    Mud_App app;
    check(Mud_init(&app, NULL) == MUD_CONTINUE);
    const MudSDLBackend_DebugStats* stats = MudSDLBackend_getDebugStats();

    MudSDLBackend_FontData* fontData = MudSDLBackend_openFontData(NULL, FONT_PATH, 10.0, MULTI_LINE);
    text.text.fontData = fontData;
    MudSDLBackend_TextureData* textureData = MudSDLBackend_createTexture(NULL, IMG_PATH, true);
    textureQuad.textureQuad.textureData = textureData;

    Mud_LayoutBox box = {
        .isActive = true,
        .renderRect = {0, 345, 10, 0}
    };
    app.scene.sceneLayout.layoutBoxes = &box;
    app.scene.sceneLayout.numLayoutBox = 1;


    MudSDLBackend_resetDebugStats();
    box.renderFunction = renderFilledQuad;
    atomic_store(&app.rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);
    Mud_render(&app);
    check(memcmp(&stats->primitivesRendered[0], &filledQuad, sizeof(Mud_FilledQuad)) == 0);


    MudSDLBackend_resetDebugStats();
    box.renderFunction = renderBorderQuad;
    atomic_store(&app.rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);
    Mud_render(&app);
    check(memcmp(&stats->primitivesRendered[0], &borderQuad, sizeof(Mud_BorderQuad)) == 0);


    MudSDLBackend_resetDebugStats();
    box.renderFunction = renderText;
    atomic_store(&app.rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);
    Mud_render(&app);
    check(memcmp(&stats->primitivesRendered[0], &text, sizeof(Mud_Text)) == 0);

    MudSDLBackend_resetDebugStats();
    box.renderFunction = renderTextureQuad;
    atomic_store(&app.rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);
    Mud_render(&app);
    check(memcmp(&stats->primitivesRendered[0], &textureQuad, sizeof(Mud_TextureQuad)) == 0);

    MudSDLBackend_closeFontData(text.text.fontData);
    MudSDLBackend_destroyTexture(textureQuad.textureQuad.textureData);
    free(fontData);
    free(textureData);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
}
