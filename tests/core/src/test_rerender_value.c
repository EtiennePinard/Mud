#include <stdatomic.h>
#include <threads.h>
#include <stdio.h>

#include "../../../mud/include/mud.h"
#include "../test_backend/include/test_backend.h"

#include "test_utils.h"

static Mud_App* otherThreadApp = NULL;

int rerenderThread(void* arg) {
    (void)arg;

    atomic_store(
        &otherThreadApp->rerenderLayoutNextFrame,
        MUD_OTHER_THREAD_RERENDER
    );

    return 0;
}


static int prepareCount = 0;
static int renderCount = 0;
static int finishCount = 0;

Mud_AppResult test_prepare(Mud_Color clearColor) {
    (void)clearColor;
    prepareCount++;

    if (otherThreadApp) {
        thrd_t t;
        int ret = thrd_create(&t, rerenderThread, NULL);
        if (ret != thrd_success) {
            fprintf(stderr, "Error when creating thread, return value %d\n", ret);
            exit(EXIT_FAILURE);
        }
        thrd_join(t, NULL);
    }

    return MUD_CONTINUE;
}

Mud_AppResult test_renderPrimitive(Mud_Primitive* primitive) {
    (void)primitive;
    renderCount++;
    return MUD_CONTINUE;
}

Mud_AppResult test_finish() {
    finishCount++;
    return MUD_CONTINUE;
}

static MudTestBackend_Options backendOptions = {
    .prepareRender = test_prepare,
    .renderPrimitive = test_renderPrimitive,
    .finishRender = test_finish
};

void resetCounters() {
    prepareCount = 0;
    renderCount = 0;
    finishCount = 0;
}

void test_noRerender(Mud_App* app) {
    resetCounters();
    atomic_store(&app->rerenderLayoutNextFrame, MUD_NO_RERENDER);

    Mud_render(app);

    check(prepareCount == 0);
    check(renderCount == 0);
    check(finishCount == 0);
    check(atomic_load(&app->rerenderLayoutNextFrame) == MUD_NO_RERENDER);
}

void test_mainThreadRerender(Mud_App* app) {
    resetCounters();
    atomic_store(&app->rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);

    Mud_render(app);

    check(prepareCount == 1);
    check(renderCount == 0);
    check(finishCount == 1);
    check(atomic_load(&app->rerenderLayoutNextFrame) == MUD_NO_RERENDER);
}

void test_otherThreadRerender(Mud_App* app) {
    resetCounters();
    atomic_store(&app->rerenderLayoutNextFrame, MUD_OTHER_THREAD_RERENDER);

    Mud_render(app);

    check(prepareCount == 1);
    check(renderCount == 0);
    check(finishCount == 1);
    check(atomic_load(&app->rerenderLayoutNextFrame) == MUD_NO_RERENDER);
}

void test_midRenderOtherThreadRerender(Mud_App* app) {
    resetCounters();

    // Triggering the other thread
    otherThreadApp = app;

    atomic_store(
        &app->rerenderLayoutNextFrame,
        MUD_MAIN_THREAD_RERENDER
    );

    Mud_render(app);

    check(prepareCount == 1);
    check(renderCount == 0);
    check(finishCount == 1);
    check(atomic_load(&app->rerenderLayoutNextFrame) == MUD_MAIN_THREAD_RERENDER);

    otherThreadApp = NULL;
}


int main(void) {
    Mud_App app;
    check(Mud_init(&app, &backendOptions) == MUD_CONTINUE);

    test_noRerender(&app);
    test_mainThreadRerender(&app);
    test_otherThreadRerender(&app);
    test_midRenderOtherThreadRerender(&app);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    return 0;
}
