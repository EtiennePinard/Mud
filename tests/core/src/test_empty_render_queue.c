#include "../../../mud/include/mud.h"
#include "test_utils.h"

int main(void) {
    Mud_App app;
    Mud_init(&app, NULL);

    atomic_store(&app.rerenderLayoutNextFrame, MUD_MAIN_THREAD_RERENDER);
    check(Mud_render(&app) == MUD_CONTINUE);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    return 0;
}
