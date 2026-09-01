#include "widgets_utilities.h"

Mud_LayoutBox MudWidgets_makeWidget(Mud_Rect rect,
                                    Mud_RenderFunction renderFunction,
                                    void* renderData) {
    return (Mud_LayoutBox){ .isActive = true,
                            .renderRect = rect,
                            .renderFunction = renderFunction,
                            .renderData = renderData };
}

bool MudWidgets_isHovered(const Mud_LayoutBox* box, const Mud_App* app) {
    return Mud_pointInRect(app->events.mouseState.mousePoint, box->renderRect);
}

bool MudWidgets_isPressed(const Mud_LayoutBox* box, const Mud_App* app) {
    return MudWidgets_isHovered(box, app) &&
           (app->events.mouseState.holdingLeftMouseButton ||
            app->events.mouseState.holdingRightMouseButton);
}
