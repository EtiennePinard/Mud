#ifndef F31701C3_10A8_4816_B933_949C8C9D843E
#define F31701C3_10A8_4816_B933_949C8C9D843E

#include <stdbool.h>
#include <stddef.h>

#include "mud_events.h"
#include "mud_types.h"

typedef Mud_AppResult (*Mud_RenderFunction)(Mud_Rect, Mud_App *);

typedef struct Mud_LayoutBox {
  bool isActive;
  Mud_Rect renderRect;
  Mud_RenderFunction renderFunction;
  Mud_EventCallback onMouseButtonDown;
  Mud_EventCallback onMouseButtonUp;
  Mud_EventCallback onMouseEntered;
  Mud_EventCallback onMouseHovered;
  Mud_EventCallback onMouseExited;
  Mud_EventCallback onMouseWheelScrolled;
} Mud_LayoutBox;

typedef struct Mud_Layout {
  Mud_Color bgColor;
  Mud_LayoutBox *layoutBoxes;
  size_t numLayoutBox;
} Mud_Layout;

/**
 * @brief Gets the window rectangle. This window is not necessarily a GUI
 * window, it is the size of the thing that the Mud_Layout will render to. The
 * width and height of the rect is the window size, and this rectangle is
 * positioned at x = 0, y = 0.
 *
 * @return Mud_Rect The window's rectangle.
 */
MUD_API Mud_Rect Mud_getWindowRect();

#endif /* F31701C3_10A8_4816_B933_949C8C9D843E */
