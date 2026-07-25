/**
 * @file mud_widgets.h
 * @brief Reusable user interface widgets built on top of Mud.
 *
 * MudWidgets provides a small collection of reusable controls such as
 * buttons, labels and checkboxes.
 *
 * Each widget is implemented as a standard Mud_LayoutBox whose render
 * function is supplied automatically by the library. Widget appearance
 * is controlled through style structures supplied by the application.
 *
 * The widget style structures are never copied internally. Instead,
 * MudWidgets stores a pointer to the supplied style inside the layout
 * box's renderData field.
 *
 * Consequently, every style object must remain valid for as long as
 * the corresponding layout box exists.
 *
 * Typical usage:
 *
 * @code
 * static MudWidget_ButtonStyle playButton = {
 *     ...
 * };
 *
 * layoutBoxes[0] =
 *     MudWidget_makeButton(rect, &playButton);
 *
 * layoutBoxes[0].onMouseButtonUp = startGame;
 * layoutBoxes[0].onMouseEntered  = MudCommon_eventRequestRerender;
 * layoutBoxes[0].onMouseExited   = MudCommon_eventRequestRerender;
 * @endcode
 *
 */

#ifndef UUID_9990EE00_0294_47CB_AB3F_456109ED385C_H
#define UUID_9990EE00_0294_47CB_AB3F_456109ED385C_H

#include <mud.h>

typedef struct MudWidget_LabelStyle {
    char* text;
    void* fontData;
    Mud_Color textColor;
} MudWidget_LabelStyle;
Mud_LayoutBox MudWidget_makeLabels(Mud_Rect rect,
                                   const MudWidget_LabelStyle* style);

typedef struct MudWidget_ButtonStyle {
    char* text;
    void* fontData;
    Mud_Color idleColor;
    Mud_Color hoverColor;
    Mud_Color pressedColor;
    Mud_Color borderColor;
    Mud_Color textColor;
    float borderThickness;

} MudWidget_ButtonStyle;
Mud_LayoutBox MudWidget_makeButton(Mud_Rect rect,
                                   const MudWidget_ButtonStyle* style);

typedef struct MudWidget_CheckboxStyle {
    bool* checked;
    char* label;
    void* fontData;
    Mud_Color backgroundColor;
    Mud_Color checkColor;
    Mud_Color borderColor;
    Mud_Color textColor;
    float borderThickness;
} MudWidget_CheckboxStyle;
Mud_LayoutBox MudWidget_makeCheckbox(Mud_Rect rect,
                                     const MudWidget_CheckboxStyle* style);

#endif /* UUID_9990EE00_0294_47CB_AB3F_456109ED385C_H */
