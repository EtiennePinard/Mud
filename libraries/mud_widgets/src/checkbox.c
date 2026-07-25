#include "../include/mud_widgets.h"
#include "widgets_utilities.h"

#include <mud_common.h>

Mud_AppResult MudWidget_renderCheckbox(Mud_LayoutBox* box, Mud_App* app) {
    const MudWidget_CheckboxStyle* style = box->renderData;

    Mud_Rect checkRect = box->renderRect;

    checkRect.width = checkRect.height;

    MudCommon_returnOnFailure(
        MudCommon_addFilledQuad(app, checkRect, style->backgroundColor));

    MudCommon_returnOnFailure(MudCommon_addBorderQuad(
        app, checkRect, style->borderColor, style->borderThickness));

    if (*style->checked) {

        Mud_Rect mark = MudCommon_scaleRectToParent(checkRect, 0.6f, 0.6f);

        mark = MudCommon_centerRectInContainer(mark, checkRect);

        MudCommon_returnOnFailure(
            MudCommon_addFilledQuad(app, mark, style->checkColor));
    }

    Mud_Rect labelRect = box->renderRect;

    labelRect.x += checkRect.width + 8;
    labelRect.width -= checkRect.width + 8;

    return MudCommon_addText(app, labelRect, style->label, style->textColor,
                             style->fontData);
}

Mud_LayoutBox MudWidget_makeCheckbox(Mud_Rect rect,
                                     const MudWidget_CheckboxStyle* style) {
    return MudWidgets_makeWidget(rect, MudWidget_renderCheckbox, (void*)style);
}