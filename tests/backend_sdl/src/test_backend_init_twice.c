#include "../../../mud/include/mud.h"

#include "test_utils.h"

int main(void) {
    Mud_App app;

    check(Mud_init(&app, NULL) == MUD_CONTINUE);

    // Calling init again should not crash
    check(Mud_init(&app, NULL) == MUD_CONTINUE);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);

    return 0;
}