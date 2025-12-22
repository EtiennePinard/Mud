#include "../../../mud/include/mud.h"
#include "../../../backends/sdl_backend/include/sdl_backend.h" 

#include "test_utils.h"

int main(void) {
    Mud_App app;
    check(Mud_init(&app, NULL) == MUD_CONTINUE);
    const MudSDLBackend_DebugStats* stats = MudSDLBackend_getDebugStats();
    MudSDLBackend_resetDebugStats();

    atomic_store(&app.rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);
    check(Mud_render(&app) == MUD_CONTINUE);

    check(stats->framesStarted == 1);
    check(stats->framesFinished == 1);
    check(stats->nb_primitivesRendered == 0);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    return 0;
}
