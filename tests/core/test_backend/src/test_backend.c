#include <stddef.h>

#include "../include/test_backend.h"

static MudTestBackend_Options* backendOptions = NULL;

Mud_AppResult MudBackend_init(void* options) {
    backendOptions = (MudTestBackend_Options*)options;
    if (backendOptions && backendOptions->init) {
        return backendOptions->init(options);
    }
    return MUD_CONTINUE;
}

void MudBackend_terminate() {
    if (backendOptions && backendOptions->terminate) {
        backendOptions->terminate();
    }
}

Mud_AppResult MudBackend_prepareRender(Mud_Color clearColor) {
    if (backendOptions && backendOptions->prepareRender) {
        return backendOptions->prepareRender(clearColor);
    }
    return MUD_CONTINUE;
}

Mud_AppResult MudBackend_renderPrimitive(Mud_Primitive* primitive) {
    if (backendOptions && backendOptions->renderPrimitive) {
        return backendOptions->renderPrimitive(primitive);
    }
    return MUD_CONTINUE;
}

Mud_AppResult MudBackend_finishRender() {
    if (backendOptions && backendOptions->finishRender) {
        return backendOptions->finishRender();
    }
    return MUD_CONTINUE;
}

Mud_AppResult MudBackend_popEvent(Mud_Event* outEvent) {
    if (backendOptions && backendOptions->popEvent) {
        return backendOptions->popEvent(outEvent);
    }
    return MUD_CONTINUE;
}

Mud_Rect MudBackend_getWindowRect() {
    if (backendOptions && backendOptions->getWindowRect) {
        return backendOptions->getWindowRect();
    }
    return (Mud_Rect) { 0, 0, 0, 0 };
}
