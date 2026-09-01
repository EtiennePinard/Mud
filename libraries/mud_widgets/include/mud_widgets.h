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
 * static MudWidget_ButtonStyle playButton = MudWidget_initButtonStyle(...)
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
#include <stdlib.h>

typedef void (*DestroyFontData)(void* fontData);

typedef struct MudWidget_LabelStyle {
    char* text;
    void* fontData;
    Mud_Color textColor;
} MudWidget_LabelStyle;
Mud_LayoutBox MudWidget_makeLabel(Mud_Rect rect,
                                   const MudWidget_LabelStyle* style);

typedef struct MudWidget_ButtonStyle {
    char* text;
    void* fontData;
    Mud_Color idleColor;
    Mud_Color hoverColor;
    Mud_Color pressedColor;
    Mud_Color textColor;
    Mud_Color borderColor;
    float borderThickness;

} MudWidget_ButtonStyle;
Mud_LayoutBox MudWidget_makeButton(Mud_Rect rect,
                                   const MudWidget_ButtonStyle* style);

/**
 * @brief Initializes the non pointer fields of the ButtonStyle struct.
 *
 * @param buttonStyle The buttonStyle to initialize.
 * @param idleColor The color when no interactions are happening.
 * @param hoverColor The color when hovered.
 * @param pressedColor The color when pressed.
 * @param textColor The color of the button's label.
 * @param borderColor The border color of the button.
 * @param borderThickness The thickness of the border.
 */
static inline void
MudWidget_initButtonStyle(MudWidget_ButtonStyle* buttonStyle,
                          Mud_Color idleColor, Mud_Color hoverColor,
                          Mud_Color pressedColor, Mud_Color textColor,
                          Mud_Color borderColor, float borderThickness) {
    buttonStyle->idleColor = idleColor;
    buttonStyle->hoverColor = hoverColor;
    buttonStyle->pressedColor = pressedColor;
    buttonStyle->borderColor = borderColor;
    buttonStyle->textColor = textColor;
    buttonStyle->borderThickness = borderThickness;
}

/**
 * @brief Frees the text and font data parameters in the button style struct.
 * The fontData is first run passed to the destroyFontDataFunction.
 *
 * @param buttonStyle The button style to free.
 * @param destroyFontDataFunction A function that closes the current font data.
 */
static inline void
MudWidget_destroyButtonStyle(MudWidget_ButtonStyle* buttonStyle,
                             DestroyFontData destroyFontDataFunction) {
    free(buttonStyle->text);
    destroyFontDataFunction(buttonStyle->fontData);
    free(buttonStyle->fontData);
}

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
