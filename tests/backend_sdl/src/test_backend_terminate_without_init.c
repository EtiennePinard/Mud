#include "../../../mud/include/mud.h"

#include "test_utils.h"

int main(void) {
    Mud_App app;

    // Should not crash
    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);

    return 0;
}