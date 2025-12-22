#include "../../../mud/include/mud.h"

int main(void) {
    Mud_App app;
    Mud_init(&app, NULL);

    app.scene.data = NULL;
    Mud_render(&app);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    return 0;
}
