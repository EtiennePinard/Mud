# WARNING: This project contains AI code, mainly with tests and documentation

# Mud

Mud (because Clay was taken) is a UI layout and event framework written in C.

It separates **what to render** from **how to render it**:

- Mud handles:
  - UI layout
  - Event dispatching
  - Render command generation

- Backends handle:
  - Drawing primitives (SDL, OpenGL, etc.)
  - Windowing
  - Input collection

This design makes Mud:
- Backend-agnostic
- Easy to test
- Portable across platforms

---

# Installation
For those who simply want to install Mud on their system.
```bash
git clone https://github.com/EtiennePinard/Mud.git
cd Mud
make
sudo make install
```

# Minimal Example

A minimal Mud application does the following thing:
 1. Initialize the Mud API, backend, and the scene
 2. Handles events one by one
 3. Renders the layout when needed
 4. Terminates

The main function of any Mud app should be very similar and follow this structure:

```c
#include <stdlib.h>
#include "mud.h"

int main(void) {
    Mud_App app;
    if (Mud_init(&app, &options) != MUD_CONTINUE) return EXIT_FAILURE;
    initScene(&app); // This is a user defined function to initialize its custom scene

    Mud_AppResult result;
    while (1) {
        if ((result = Mud_handleEvents(&app)) != MUD_CONTINUE) break;
        if ((result = Mud_render(&app)) != MUD_CONTINUE) break;
    }

    Mud_terminate(&app, result);
    return EXIT_SUCCESS;
}
````

See `examples/` folder for complete examples.

---

# Architecture Overview

Mud is built around three core concepts:

### 1. Layout

Defines *where* things are on the screen using layout boxes.

### 2. Events

Mud processes backend-provided events and dispatches:

* Global app events
* Local layout box events

### 3. Rendering

Mud generates a queue of primitives:

* Filled quads
* Border quads
* Textured quads
* Text

Backends consume this queue and render it.

---

# Repository Structure

```
mud/         # Core Mud library
backends/    # Rendering backends (SDL, test backend, etc.)
examples/    # Example applications
tests/       # Core and backend tests
docs/        # Documentation
```

---

# Documentation
* `docs/mud.md` 
  Contains a description of the Mud library structure

* `docs/backend_contract.md`
  Defines the interface between Mud and rendering backends

* `docs/backend_test_guidelines.md`
  Explains how to test backend implementations

---

# Building

Mud uses Make.

### Show available commands:

```sh
make help
```

### Build everything:

```sh
make
```

---

# Running Examples

Run the SDL hello world example:

```sh
make hello-worlds
```

---

# Running Tests

Run all tests:

```sh
make tests
```

---

# Backends

Mud is backend-agnostic. Current backends include:

* Test backend (for unit testing)
* SDL backend (first real rendering backend)

You can implement your own backend by following the backend contracts outline in `docs/backend_contract.md`

---

# Design Philosophy

Mud is designed to:

* Be simple to integrate
* Avoid hidden state
* Make rendering explicit
* Be fully testable
* Stay portable and minimal

---

# Contributing

Contributions are welcome, especially:

* New backends (OpenGL, Vulkan, Raylib, etc.)
* Improvements to layout system
* Better testing infrastructure
* Documentation improvements

Before implementing a backend, read:

* `docs/backend_contract.md`
* `docs/backend_test_guidelines.md`

---

# Status

Mud is currently under active development.

Core features implemented:

* Render queue
* Event system
* Layout boxes
* Backend abstraction

Next steps:

* Backend test suite expansion
* Additional backends
* More complex layout features

---

# License

Mud uses the GNU general public license version 3. See `LICENSE` for the full document.
