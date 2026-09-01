#include "../include/mud_widgets.h"
#include "widgets_utilities.h"

#include <mud_common.h>

Mud_AppResult MudWidget_renderLabel(Mud_LayoutBox* box, Mud_App* app) {
    const MudWidget_LabelStyle* style = box->renderData;

    return MudCommon_addText(app, box->renderRect, style->text,
                             style->textColor, style->fontData);
}

Mud_LayoutBox MudWidget_makeLabel(Mud_Rect rect,
                                  const MudWidget_LabelStyle* style) {
    return MudWidgets_makeWidget(rect, MudWidget_renderLabel, (void*)style);
}
