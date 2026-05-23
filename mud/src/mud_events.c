#include "../include/mud_backend.h"

Mud_AppResult Mud_handleEvents(Mud_App *app) {
  Mud_Event event;
  event.type = MUD_EVENT_NONE;
  if (MudBackend_popEvent(&event) != MUD_CONTINUE) {
    // Backend error, terminating the app
    return MUD_TERMINATE_WITH_FAILURE;
  }

  Mud_AppResult result = MUD_CONTINUE;
  Mud_Layout layout;
  Mud_Point previousMousePoint;
  switch (event.type) {
  case MUD_EVENT_NONE:
    // If there no event to handle so return
    return result;
  case MUD_EVENT_QUIT:
    if (app->events.onQuit) {
      result = app->events.onQuit(&event, app);
    } else {
      // The default quit event is to terminate with success
      result = MUD_TERMINATE_WITH_SUCCESS;
    }
    break;
  case MUD_EVENT_KEY_DOWN:
    if (app->events.onKeyDown) {
      result = app->events.onKeyDown(&event, app);
    }
    break;
  case MUD_EVENT_KEY_UP:
    if (app->events.onKeyUp) {
      result = app->events.onKeyUp(&event, app);
    }
    break;
  case MUD_EVENT_WINDOW_RESIZE:
    if (app->events.onWindowResize) {
      result = app->events.onWindowResize(&event, app);
    }
    break;
  case MUD_EVENT_MOUSE_MOVE:
    previousMousePoint = app->events.mouseState.mousePoint;
    // Start by updating mouse state
    app->events.mouseState.mousePoint =
        (Mud_Point){.x = event.mouseMove.x, .y = event.mouseMove.y};
    app->events.mouseState.holdingLeftMouseButton =
        event.mouseMove.pressedButtonFlag & MUD_LEFT_BUTTON_MASK;
    app->events.mouseState.holdingRightMouseButton =
        event.mouseMove.pressedButtonFlag & MUD_RIGHT_BUTTON_MASK;

    // Handling layout events
    layout = app->scene.sceneLayout;
    // Reverse iteration since the last layout boxes have the highest z-order
    for (size_t index = layout.numLayoutBox; index-- > 0;) {
      Mud_LayoutBox layoutBox = layout.layoutBoxes[index];
      if (!layoutBox.isActive)
        continue;
      bool isLastPointInsideRect =
          Mud_pointInRect(previousMousePoint, layoutBox.renderRect);
      bool isCurrentPointInsideRect = Mud_pointInRect(
          app->events.mouseState.mousePoint, layoutBox.renderRect);
      if (!isLastPointInsideRect && isCurrentPointInsideRect) {
        if (layoutBox.onMouseEntered)
          result = layoutBox.onMouseEntered(&event, app);
        if (result != MUD_CONTINUE)
          return result;
      }
      if (isCurrentPointInsideRect) {
        if (layoutBox.onMouseHovered)
          result = layoutBox.onMouseHovered(&event, app);
        if (result != MUD_CONTINUE)
          return result;
      }
      if (isLastPointInsideRect && !isCurrentPointInsideRect) {
        if (layoutBox.onMouseExited)
          result = layoutBox.onMouseExited(&event, app);
        if (result != MUD_CONTINUE)
          return result;
      }
      // Only the highest z-order box consumes the event
      break;
    }
    break;
  case MUD_EVENT_MOUSE_BUTTON_DOWN:
  case MUD_EVENT_MOUSE_BUTTON_UP:
    // Start by updating mouse state
    app->events.mouseState.mousePoint =
        (Mud_Point){.x = event.mouseButton.x, .y = event.mouseButton.y};
    if (event.mouseButton.mouseButtonIndex == MUD_LEFT_MOUSE_BUTTON) {
      app->events.mouseState.holdingLeftMouseButton =
          event.type == MUD_EVENT_MOUSE_BUTTON_DOWN;
    } else if (event.mouseButton.mouseButtonIndex == MUD_RIGHT_MOUSE_BUTTON) {
      app->events.mouseState.holdingRightMouseButton =
          event.type == MUD_EVENT_MOUSE_BUTTON_DOWN;
    }
    __attribute__((fallthrough));
    // Kinda gross to have a case statement not at the beginning
    // but it removes duplicated code so we take it
  case MUD_EVENT_MOUSE_WHEEL:
    // Handling layout events
    layout = app->scene.sceneLayout;
    // Reverse iteration since the last layout boxes have the highest z-order
    for (size_t index = layout.numLayoutBox; index-- > 0;) {
      Mud_LayoutBox layoutBox = layout.layoutBoxes[index];
      if (!layoutBox.isActive)
        continue;
      bool isCurrentPointInsideRect = Mud_pointInRect(
          app->events.mouseState.mousePoint, layoutBox.renderRect);
      if (isCurrentPointInsideRect) {
        switch (event.type) {
        case MUD_EVENT_MOUSE_BUTTON_DOWN:
          if (layoutBox.onMouseButtonDown)
            result = layoutBox.onMouseButtonDown(&event, app);
          goto END_INNER_SWITCH_EVENT_TYPE;
        case MUD_EVENT_MOUSE_BUTTON_UP:
          if (layoutBox.onMouseButtonUp)
            result = layoutBox.onMouseButtonUp(&event, app);
          goto END_INNER_SWITCH_EVENT_TYPE;
        case MUD_EVENT_MOUSE_WHEEL:
          if (layoutBox.onMouseWheelScrolled)
            result = layoutBox.onMouseWheelScrolled(&event, app);
          goto END_INNER_SWITCH_EVENT_TYPE;
        default:
          __builtin_unreachable();
        }
      END_INNER_SWITCH_EVENT_TYPE:
        if (result != MUD_CONTINUE)
          return result;
      }
      // Only the highest z-order box consumes the event
      break;
    }
    break;
  default:
    break;
  }

  return result;
}
