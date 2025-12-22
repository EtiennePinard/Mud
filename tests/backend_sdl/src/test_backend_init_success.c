#include "../../../mud/include/mud.h"

#include "test_utils.h"

int main(void) {
    Mud_App app;
    // Backend should be able to initialized with NULL passed to options pointer
    check(Mud_init(&app, NULL) == MUD_CONTINUE);
    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    return 0;
}
