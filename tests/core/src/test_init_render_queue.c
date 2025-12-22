#include "../../../mud/include/mud.h"
#include "test_utils.h"

int main(void) {
    Mud_App app;
    Mud_init(&app, NULL);

    check(app.renderQueue.count == 0);
    check(app.renderQueue.commands != NULL);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    return 0;
}
