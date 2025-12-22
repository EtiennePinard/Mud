# Mud Backend Test Guidelines

This document defines how to test a Mud backend for compliance with the Mud Backend Contract.

Each backend must provide a test suite validating:
- Correct lifecycle behavior
- Correct rendering behavior
- Correct event handling
- Stability and error handling

These tests are **backend-specific** and must be implemented using the backend's native API.

---

# 1. General Testing Principles

## 1.1 Black-box testing

Tests should treat Mud as a black box:
- Do NOT modify Mud internals
- Do NOT override backend functions
- Use the real backend implementation

---

## 1.2 Determinism

Tests must:
- Produce deterministic results
- Avoid timing-based assertions
- Avoid relying on real user input

---

## 1.3 Minimal apps

Each test should:
- Initialize a `Mud_App`
- Configure a minimal scene or render queue
- Call `Mud_render` and/or `Mud_handleEvents`
- Validate observable behavior

---

## 1.4 Headless testing

Backends should support:
- Hidden window mode OR
- Offscreen rendering

This allows automated testing without UI interaction.

---

# 2. Initialization Tests

## 2.1 Init with NULL options

**Goal:** Ensure backend handles default initialization.

**Test:**
- Call `Mud_init(&app, NULL)`
- Expect `MUD_CONTINUE`

---

## 2.2 Init with valid options

**Goal:** Ensure backend correctly uses configuration.

**Test:**
- Provide valid backend options
- Expect successful initialization

---

## 2.3 Init failure handling

**Goal:** Ensure backend reports failure correctly.

**Test:**
- Pass invalid options (e.g., invalid window size)
- Expect != `MUD_CONTINUE`

---

# 3. Frame Lifecycle Tests

## 3.1 Empty frame

**Goal:** Ensure backend handles frames with no primitives.

**Test:**
- No primitives in queue
- Call `Mud_render`
- Expect:
  - No crash
  - Frame completes successfully

---

## 3.2 Single primitive

**Goal:** Ensure basic rendering works.

**Test:**
- Submit one primitive
- Call `Mud_render`
- Verify:
  - Primitive appears (visually OR via backend logging)

---

## 3.3 Multiple primitives

**Goal:** Ensure multiple primitives render correctly.

**Test:**
- Submit N primitives
- Verify all are rendered

---

## 3.4 Z-order correctness

**Goal:** Ensure rendering order is preserved.

**Test:**
- Overlapping primitives
- Later primitive must appear on top

---

## 3.5 Re-render behavior

**Goal:** Ensure backend integrates with Mud rerender logic.

**Test:**
- Trigger multiple renders
- Ensure no duplicate frames or crashes

---

# 4. Primitive Rendering Tests

Each primitive type must be tested.

---

## 4.1 Filled Quad

**Test:**
- Render rectangle with known color
- Verify correct position and color

---

## 4.2 Border Quad

**Test:**
- Render with border thickness
- Verify border appearance

---

## 4.3 Textured Quad

**Test:**
- Render texture
- Verify:
  - Texture appears
  - Tint is applied (if supported)

---

## 4.4 Text

**Test:**
- Register font
- Render text
- Verify:
  - Text appears
  - Correct font is used

---

# 5. Event Handling Tests

## 5.1 No event

**Goal:** Ensure empty queue behavior.

**Test:**
- Call `Mud_handleEvents`
- Expect no callbacks triggered

---

## 5.2 Key events

**Test:**
- Simulate key press
- Verify callback is triggered

---

## 5.3 Mouse movement

**Test:**
- Simulate mouse move
- Verify:
  - Position updated
  - Hover logic works

---

## 5.4 Mouse buttons

**Test:**
- Simulate click
- Verify:
  - Button state updates
  - Callbacks triggered

---

## 5.5 Mouse wheel

**Test:**
- Simulate scroll
- Verify callback execution

---

## 5.6 Window resize

**Test:**
- Simulate resize
- Verify callback execution

---

# 6. Error Handling Tests

## 6.1 Render failure

**Goal:** Ensure backend errors propagate.

**Test:**
- Force backend failure (if possible)
- Verify Mud exits render loop

---

## 6.2 Resource failure

**Test:**
- Load invalid font or texture
- Ensure:
  - No crash
  - Graceful failure

---

# 7. Stability Tests

## 7.1 Stress test

**Test:**
- Render many primitives (e.g., 10,000)
- Ensure:
  - No crash
  - Acceptable performance

---

## 7.2 Repeated init/terminate

**Test:**
- Initialize and terminate multiple times
- Ensure no leaks or crashes

---

# 8. Summary

A compliant backend must:

* Pass all initialization tests
* Respect frame lifecycle behavior
* Render all primitives correctly
* Provide valid events
* Handle errors gracefully

These tests ensure compatibility with Mud and portability across backends.
