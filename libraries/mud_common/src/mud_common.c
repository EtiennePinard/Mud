#include "../include/mud_common.h"

Mud_AppResult MudCommon_addFilledQuad(Mud_App* app, Mud_Rect renderRect,
                                      Mud_Color color) {
    Mud_Primitive filledQuad = { .filledQuad = { .type =
                                                     MUD_PRIMITIVE_FILLED_QUAD,
                                                 .renderRect = renderRect,
                                                 .color = color } };
    return Mud_addPrimitive(app, &filledQuad);
}

Mud_AppResult MudCommon_addBorderQuad(Mud_App* app, Mud_Rect renderRect,
                                      Mud_Color color, float borderThickness) {
    Mud_Primitive borderQuad = { .borderQuad = {
                                     .type = MUD_PRIMITIVE_BORDER_QUAD,
                                     .renderRect = renderRect,
                                     .color = color,
                                     .borderThickness = borderThickness } };
    return Mud_addPrimitive(app, &borderQuad);
}

Mud_AppResult MudCommon_addText(Mud_App* app, Mud_Rect renderRect,
                                const char* text, Mud_Color color,
                                void* fontData) {
    Mud_Primitive textPrimitive = { .text = { .type = MUD_PRIMITIVE_TEXT,
                                              .renderRect = renderRect,
                                              .text = text,
                                              .color = color,
                                              .fontData = fontData } };
    return Mud_addPrimitive(app, &textPrimitive);
}

Mud_AppResult MudCommon_addTextureQuad(Mud_App* app, Mud_Rect renderRect,
                                       void* textureData) {
    Mud_Primitive textureQuad = { .textureQuad = {
                                      .type = MUD_PRIMITIVE_TEXTURED_QUAD,
                                      .renderRect = renderRect,
                                      .textureData = textureData } };
    return Mud_addPrimitive(app, &textureQuad);
}

Mud_BoxEventResult MudCommon_boxEventRequestRerender(Mud_Event* event, Mud_App* app) {
    (void)event;
    MudCommon_requestRenderFromMainThread(app);
    return MUD_HANDLED_EVENT;
}