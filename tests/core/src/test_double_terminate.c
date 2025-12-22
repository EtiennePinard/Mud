#include "../../../mud/include/mud.h"

int main(void) {
    Mud_App app;
    Mud_init(&app, NULL);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS); // Mud_terminate is idempotent

    return 0;
}
