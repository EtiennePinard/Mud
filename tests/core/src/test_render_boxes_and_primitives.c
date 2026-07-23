#include <string.h>
#include <stdio.h>

#include "../../../mud/include/mud.h"
#include "../test_backend/include/test_backend.h"

#include "test_utils.h"

#define MAX_CAPTURED 32
static Mud_Primitive* captured[MAX_CAPTURED];
static size_t capturedCount = 0;
static int prepareCount = 0;
static int finishCount = 0;

Mud_AppResult test_renderPrimitive(Mud_Primitive* primitive) {
    check(capturedCount < MAX_CAPTURED);
    captured[capturedCount++] = primitive;
    return MUD_CONTINUE;
}

Mud_AppResult test_prepare(Mud_Color clearColor) {
    (void)clearColor;
    prepareCount++;
    return MUD_CONTINUE;
}

Mud_AppResult test_finish() {
    finishCount++;
    return MUD_CONTINUE;
}

static MudTestBackend_Options backendOptions = {
    .renderPrimitive = test_renderPrimitive,
    .prepareRender = test_prepare,
    .finishRender = test_finish
};

#define FONT_DATA (42)
static int fontData = FONT_DATA;
#define TEXTURE_QUAD_DATA (137)
static int textureData = TEXTURE_QUAD_DATA;

static int textRendered = 0; // Lowest z-order
static int filledQuadRendered = 0; // Middle low z-order
static int borderQuadRendered = 0; // Middle high z-order
static int textureQuadRendered = 0; // Highest z-order
Mud_AppResult renderTextLayoutBox(Mud_LayoutBox* box, Mud_App* app) {
    check(textRendered == 0);
    check(filledQuadRendered == 0);
    check(borderQuadRendered == 0);
    check(textureQuadRendered == 0);
    textRendered++;
    Mud_Primitive text = {
        .text = {
            .type = MUD_PRIMITIVE_TEXT,
            .renderRect = box->renderRect,
            .color = {0, 0, 0, 1},
            .text = "Mud",
            .fontData = &fontData
        }
    };
    return Mud_addPrimitive(app, &text);
}
Mud_AppResult renderFilledQuadBox(Mud_LayoutBox* box, Mud_App* app) {
    check(textRendered == 1);
    check(filledQuadRendered == 0);
    check(borderQuadRendered == 0);
    check(textureQuadRendered == 0);
    filledQuadRendered++;
    Mud_Primitive filledQuad = {
        .filledQuad = {
            .type = MUD_PRIMITIVE_FILLED_QUAD,
            .renderRect = box->renderRect,
            .color = {0, 0, 1, 0}
        }
    };
    return Mud_addPrimitive(app, &filledQuad);
}
Mud_AppResult renderBorderQuadBox(Mud_LayoutBox* box, Mud_App* app) {
    check(textRendered == 1);
    check(filledQuadRendered == 1);
    check(borderQuadRendered == 0);
    check(textureQuadRendered == 0);
    borderQuadRendered++;
    Mud_Primitive borderQuad = {
        .borderQuad = {
            .type = MUD_PRIMITIVE_BORDER_QUAD,
            .renderRect = box->renderRect,
            .color = {0, 0, 1, 1}
        }
    };
    return Mud_addPrimitive(app, &borderQuad);
}
Mud_AppResult renderTextureQuad(Mud_LayoutBox* box, Mud_App* app) {
    check(textRendered == 1);
    check(filledQuadRendered == 1);
    check(borderQuadRendered == 1);
    check(textureQuadRendered == 0);
    textureQuadRendered++;
    Mud_Primitive textureQuad = {
        .textureQuad = {
            .type = MUD_PRIMITIVE_TEXTURED_QUAD,
            .renderRect = box->renderRect,
            .textureData = &textureData
        }
    };
    return Mud_addPrimitive(app, &textureQuad);
}

void resetCounters() {
    prepareCount = finishCount = capturedCount = 0;
    textRendered = filledQuadRendered = borderQuadRendered = 0;
}

void test_renderLayoutBoxesAndPrimitives(Mud_App* app) {
    resetCounters();

    check(app->renderQueue.count == 0);
    atomic_store(&app->rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);
    Mud_render(app);

    check(prepareCount == 1);
    check(finishCount == 1);

    check(textRendered == 1);
    check(filledQuadRendered == 1);
    check(borderQuadRendered == 1);
    check(textureQuadRendered == 1);

    check(capturedCount == 4);
    check(captured[0]->type == MUD_PRIMITIVE_TEXT);
    Mud_Rect rect = { 1, 2, 3, 4 };
    check(memcmp(&captured[0]->text.renderRect, &rect, sizeof(Mud_Rect)) == 0);
    Mud_Color color = { 0, 0, 0, 1 };
    check(memcmp(&captured[0]->text.color, &color, sizeof(Mud_Color)) == 0);
    check(strncmp(captured[0]->text.text, "Mud", 5) == 0);
    check(*(int*)captured[0]->text.fontData == FONT_DATA);

    check(captured[1]->type == MUD_PRIMITIVE_FILLED_QUAD);
    rect = (Mud_Rect){ 5, 6, 7, 8 };
    check(memcmp(&captured[1]->filledQuad.renderRect, &rect, sizeof(Mud_Rect)) == 0);
    color = (Mud_Color){ 0, 0, 1, 0 };
    check(memcmp(&captured[1]->filledQuad.color, &color, sizeof(Mud_Color)) == 0);

    check(captured[2]->type == MUD_PRIMITIVE_BORDER_QUAD);
    rect = (Mud_Rect){ 9, 10, 11, 12 };
    check(memcmp(&captured[2]->borderQuad.renderRect, &rect, sizeof(Mud_Rect)) == 0);
    color = (Mud_Color){ 0, 0, 1, 1 };
    check(memcmp(&captured[2]->borderQuad.color, &color, sizeof(Mud_Color)) == 0);

    check(captured[3]->type == MUD_PRIMITIVE_TEXTURED_QUAD);
    rect = (Mud_Rect){ 13, 14, 15, 16 };
    check(memcmp(&captured[3]->textureQuad.renderRect, &rect, sizeof(Mud_Rect)) == 0);
    check(*(int*)captured[3]->textureQuad.textureData == TEXTURE_QUAD_DATA);
}

int main(void) {
    Mud_App app;
    check(Mud_init(&app, &backendOptions) == MUD_CONTINUE);

    Mud_LayoutBox layoutBoxes[4] = {
        (Mud_LayoutBox) {
            .isActive = true,
            .renderRect = {1, 2, 3, 4},
            .renderFunction = renderTextLayoutBox
        },
        (Mud_LayoutBox) {
            .isActive = true,
            .renderRect = {5, 6, 7, 8},
            .renderFunction = renderFilledQuadBox
        },
        (Mud_LayoutBox) {
            .isActive = true,
            .renderRect = {9, 10, 11, 12},
            .renderFunction = renderBorderQuadBox
        },
        (Mud_LayoutBox) {
            .isActive = true,
            .renderRect = {13, 14, 15, 16},
            .renderFunction = renderTextureQuad
        }
    };
    app.scene.sceneLayout.layoutBoxes = layoutBoxes;
    app.scene.sceneLayout.numLayoutBox = 4;

    test_renderLayoutBoxesAndPrimitives(&app);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    return 0;
}
