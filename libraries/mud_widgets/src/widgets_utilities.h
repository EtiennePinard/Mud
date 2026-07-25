#ifndef UUID_901B6BF8_2AB9_4461_9E46_63EA325F82CB_H
#define UUID_901B6BF8_2AB9_4461_9E46_63EA325F82CB_H

#include <mud.h>

Mud_LayoutBox MudWidgets_makeWidget(Mud_Rect rect,
                                    Mud_RenderFunction renderFunction,
                                    void* renderData);

bool MudWidgets_isHovered(const Mud_LayoutBox* box, const Mud_App* app);

bool MudWidgets_isPressed(const Mud_LayoutBox* box, const Mud_App* app);

#endif /* UUID_901B6BF8_2AB9_4461_9E46_63EA325F82CB_H */
