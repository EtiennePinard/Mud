# Mud Backend Contract

This document defines the contract between the Mud core library and any rendering backend.

A backend is responsible for:
- Rendering primitives
- Managing platform-specific resources (window, GPU, etc.)
- Providing input events

Mud itself is responsible for:
- Layout
- Event dispatching
- Render queue management
- Application lifecycle

Any backend must implement the functions defined in `mud_backend.h` and respect the guarantees described in this document.

---

# 1. Initialization

## Function
```c
/**
 * @brief This function is called at the start of the program
 * to initialize the backend.
 * 
 * @param options A pointer to options to the backend. The format of
 * the option data are defined by each backend. This pointer can be NULL.
 * @return Mud_AppResult If the app should continue or terminate with failure or success 
 */
Mud_AppResult MudBackend_init(void* options);
````

## Requirements

* Must initialize all backend resources (window, renderer, etc.) the first time it is called.
* Must be a idempotent when called multiple times.
* `options` is backend-defined and may be `NULL`.

## Return Value

* `MUD_CONTINUE`: Initialization succeeded
* Any other value: Initialization failed, Mud will terminate

## Guarantees

* If this function returns failure, no other backend function will be called except `MudBackend_terminate`.

---

# 2. Termination

## Function

```c
/**
 * @brief This function is called when the app terminates 
 * to terminate and cleanup the backend.
 * 
 */
void MudBackend_terminate();
```

## Requirements

* Must clean up all resources allocated in `MudBackend_init`.
* Must be safe to call even if initialization partially failed.

## Guarantees

* Called exactly once at application shutdown.

---

# 3. Frame Lifecycle

Mud performs rendering in discrete frames.

## Functions

```c
/**
 * @brief This function is called right before Mud starts 
 * rendering the layout. The backend should ready itself for rendering 
 * and clear the screen with the clear color.
 * 
 * @param clearColor The color to clear the screen with
 * @return Mud_AppResult If the app should continue or terminate with failure or success 
 */
Mud_AppResult MudBackend_prepareRender(Mud_Color clearColor);
/**
 * @brief Renders a Mud primitive to the screen.
 * 
 * @param primitive The primitive to render
 * @return Mud_AppResult If the app should continue or terminate with failure or success 
 */
Mud_AppResult MudBackend_renderPrimitive(Mud_Primitive* primitive);
/**
 * @brief Finishes the rendering process. This function is called
 * after all primitives are done rendering.
 * 
 * @param clearColor 
 * @return Mud_AppResult If the app should continue or terminate with failure or success 
 */
Mud_AppResult MudBackend_finishRender();
```

## Call Order Guarantee

For each frame:

```
prepareRender -> renderPrimitive (0..N times) -> finishRender
```

## Detailed Behavior

### 3.1 `prepareRender`

* Clears the screen using `clearColor`
* Prepares the backend for rendering
* Called once per frame

### 3.2 `renderPrimitive`

* Called once for each primitive in the render queue
* Each primitive needs to be 
* Order is **strictly preserved** (Z-order)

### 3.3 `finishRender`

* Finalizes rendering (e.g., buffer swap)
* Called once per frame

---

## Error Handling

If any function returns a value != `MUD_CONTINUE`:

* Mud immediately stops rendering
* The return value propagates up to the application
* No further backend calls are made in that frame

---

# 4. Rendering Guarantees

## 4.1 Primitive Ordering

* Primitives are submitted in strict Z-order
* Backends must render primitives **in the order received**

## 4.2 Primitive Immutability

* The `Mud_Primitive*` passed is valid only during the call
* Backends must NOT store pointers to primitives
* If needed, copy the data

---

## 4.3 Supported Primitive Types

Backends must handle all defined primitive types:

* `MUD_PRIMITIVE_FILLED_QUAD`
* `MUD_PRIMITIVE_BORDER_QUAD`
* `MUD_PRIMITIVE_TEXTURED_QUAD`
* `MUD_PRIMITIVE_TEXT`

If a backend does not support a primitive:

* It must fail gracefully (not crash)
* Prefer logging a warning

---

## 4.4 Text Rendering

Fonts are entirely specified by the backend. The user is responsible for
* Loading fonts using a backend provided function
* Managing font lifetime
* Terminating that font using a backend provided function

The backend is responsible for
* Providing the functionality to load and use fonts
* Rendering text within the provided rectangle

---

## 4.5 Texture Handling

* Texture pointers are opaque (`void*`)
* Interpretation is backend-specific
* Backend must not assume ownership unless explicitly documented

---

# 5. Event Handling

## Function

```c
/**
 * @brief Pops an event from the render queue formed by the Mud backend. If
 * there is no event to process, the backend does not modify the outEvent
 * parameter.
 * 
 * @param outEvent A pointer to place the popped event into
 * @return Mud_AppResult If the app should continue or terminate with failure or success 
 */
Mud_AppResult MudBackend_popEvent(Mud_Event* outEvent);
```

## Behavior

* Pops **at most one event per call**
* If no event is available:

  * Must set `outEvent->type = MUD_EVENT_NONE`
* Must NOT block

---

## Event Ownership

* The backend creates events
* Mud consumes them
* Backend must not retain references after returning

---

## Error Handling

* If a critical error occurs:

  * Return a value != `MUD_CONTINUE`
  * Mud will terminate the application

---

# 6. Input State Expectations

Mud maintains its own input state (`Mud_MouseState`).

Backends must:

* Provide accurate event data:

  * Mouse position
  * Button states
  * Key events

Mud will derive:

* Hover state
* Click state
* Layout interactions

---

# 7. Threading Rules

* All backend functions are called from the **main thread**
* Backends are not required to be thread-safe
* Backends must NOT:

  * Spawn uncontrolled threads
  * Modify Mud state directly

If a backend uses threads internally:

* It must fully manage synchronization

---

# 8. Required Backend Tests

Each backend must provide its own test suite verifying:

## Initialization

* Init succeeds with valid options
* Init handles NULL options
* Init failure is handled correctly

## Frame Lifecycle

* Correct call order
* Correct primitive count
* No crashes with empty render queue

## Rendering

* All primitive types render correctly
* Z-order is respected

## Events

* Events are produced correctly
* No-event case returns `MUD_EVENT_NONE`

## Stability

* No crashes on invalid input
* Graceful handling of unsupported features

---

# 9. Non-Goals of Backends

Backends must NOT:

* Perform layout calculations
* Dispatch UI events to layout boxes
* Modify Mud internal state
* Reorder primitives
* Cache Mud-owned memory

---

# 10. Summary

A valid backend must:

* Implement all required functions
* Respect call order and ownership rules
* Handle errors gracefully
* Provide correct event data
* Render primitives in order

Failure to respect this contract results in undefined behavior.

---

# Future Extensions

This contract may evolve to include:

* GPU batching guarantees
* Advanced text shaping
* High-DPI handling
* Multi-window support

