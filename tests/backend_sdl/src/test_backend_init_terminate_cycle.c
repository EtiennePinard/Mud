#include "../../../mud/include/mud.h"

#include "test_utils.h"

#define NUM_CYCLES (5)

int main(void) {
    Mud_App app;

    for (int i = 0; i < NUM_CYCLES; i++) {
        check(Mud_init(&app, NULL) == MUD_CONTINUE);
        Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    }

    return 0;
}