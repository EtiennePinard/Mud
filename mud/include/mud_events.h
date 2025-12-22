#ifndef A90E7CCD_B5DF_493A_998D_C2AA12E0FAD1
#define A90E7CCD_B5DF_493A_998D_C2AA12E0FAD1

#include "mud_types.h"
#include "mud_app.h"

typedef enum Mud_EventType {
    MUD_EVENT_NONE,
    MUD_EVENT_QUIT,
    MUD_EVENT_MOUSE_BUTTON_UP,
    MUD_EVENT_MOUSE_BUTTON_DOWN,
    MUD_EVENT_MOUSE_MOVE,
    MUD_EVENT_MOUSE_WHEEL,
    MUD_EVENT_KEY_UP,
    MUD_EVENT_KEY_DOWN,
    MUD_EVENT_WINDOW_RESIZE
} Mud_EventType;

typedef struct Mud_QuitEvent {
    Mud_EventType type;
} Mud_QuitEvent;

#define MUD_LEFT_MOUSE_BUTTON (1)
#define MUD_RIGHT_MOUSE_BUTTON (2)
#define MUD_MIDDLE_MOUSE_BUTTON (3)

typedef struct Mud_MouseButtonEvent {
    Mud_EventType type;
    // Which mouse button was pressed. Use MUD_LEFT_MOUSE_BUTTON and MUD_RIGHT_MOUSE_BUTTON
    // for left and right mouse buttons. The indices of the other mouse buttons need to be strictly 
    // higher but can be defined by the backend.
    u8 mouseButtonIndex; 
    u8 clicks; // The number of clicks did (1 for single clicks, 2 for double clicks, etc.)
    float x; // Window x coordinates
    float y; // Window y coordinates
} Mud_MouseButtonEvent;

#define MUD_LEFT_BUTTON_MASK   (1 << 0)
#define MUD_MIDDLE_BUTTON_MASK (1 << 1)
#define MUD_RIGHT_BUTTON_MASK  (1 << 2)

typedef struct Mud_MouseMoveEvent {
    Mud_EventType type;
    u32 pressedButtonFlag; // Can be AND with button mask to see if a button is pressed 
    float x; // Window x coordinates
    float y; // Window y coordinates
} Mud_MouseMoveEvent;

// Note: Copied from SDL_mouse.h file
typedef enum Mud_MouseWheelDirection {
    MUD_MOUSE_WHEEL_NORMAL,
    MUD_MOUSE_WHEEL_FLIPPED
} Mud_MouseWheelDirection;

typedef struct Mud_MouseWheelEvent {
    Mud_EventType type;
    float x;            // The amount scrolled horizontally, positive to the right and negative to the left
    float y;            // The amount scrolled vertically, positive away from the user and negative toward the user
    Mud_MouseWheelDirection direction; // Set to one of the MUD_MOUSE_WHEEL_* defines. When FLIPPED the values in X and Y will be opposite. Multiply by -1 to change them back
} Mud_MouseWheelEvent;

typedef struct Mud_KeyEvent {
    Mud_EventType type;
    u32 scancode; // The physical key pressed. Which key corresponds to which number is defined by the backend
    u32 keycode; // The key pressed by the virtual keyboard layout. Which key corresponds to which number is defined by the backend
} Mud_KeyEvent;

typedef struct Mud_WindowResizeEvent {
    Mud_EventType type;
    u32 width;
    u32 height;
} Mud_WindowResizeEvent;

typedef union Mud_Event {
    Mud_EventType type;
    Mud_QuitEvent quit;
    Mud_MouseButtonEvent mouseButton;
    Mud_MouseMoveEvent mouseMove;
    Mud_MouseWheelEvent mouseWheel;
    Mud_KeyEvent key;
    Mud_WindowResizeEvent windowResize;
} Mud_Event;

typedef Mud_AppResult(*Mud_EventCallback)(Mud_Event*, Mud_App*);

typedef struct Mud_MouseState {
    bool holdingLeftMouseButton;
    bool holdingRightMouseButton;
    Mud_Point mousePoint;
} Mud_MouseState;

typedef struct Mud_AppEvents {
    Mud_EventCallback onWindowResize;
    Mud_EventCallback onKeyDown;
    Mud_EventCallback onKeyUp;
    Mud_EventCallback onQuit;

    Mud_MouseState mouseState;
    atomic_int shouldHandleEvents;
} Mud_AppEvents;

/**
 * @brief This function is usually called every frame right before the
 * app is rendered. This function can then change the layout based
 * on user interaction, which should get reflected in the next
 * Mud_render call.
 *
 * @param app The state and events of the app
 * @return If the app should continue or terminate with failure or success
 */
MUD_API Mud_AppResult Mud_handleEvents(Mud_App* app);

#endif /* A90E7CCD_B5DF_493A_998D_C2AA12E0FAD1 */
