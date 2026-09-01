#include "../include/mud_widgets.h"
#include "widgets_utilities.h"

#include <mud_common.h>

Mud_AppResult MudWidget_renderButton(Mud_LayoutBox* box, Mud_App* app) {
    const MudWidget_ButtonStyle* style = box->renderData;

    Mud_Color fill;

    if (MudWidgets_isPressed(box, app)) fill = style->pressedColor;
    else if (MudWidgets_isHovered(box, app)) fill = style->hoverColor;
    else fill = style->idleColor;

    MudCommon_returnOnFailure(
        MudCommon_addFilledQuad(app, box->renderRect, fill));

    MudCommon_returnOnFailure(MudCommon_addBorderQuad(
        app, box->renderRect, style->borderColor, style->borderThickness));

    return MudCommon_addText(app, box->renderRect, style->text,
                             style->textColor, style->fontData);
}

Mud_LayoutBox MudWidget_makeButton(Mud_Rect rect,
                                   const MudWidget_ButtonStyle* style) {
    return MudWidgets_makeWidget(rect, MudWidget_renderButton, (void*)style);
}