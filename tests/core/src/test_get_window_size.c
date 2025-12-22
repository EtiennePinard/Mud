#include <string.h>

#include "../../../mud/include/mud.h"
#include "../test_backend/include/test_backend.h"

#include "test_utils.h"

static Mud_Rect windowRect = { 14.4, 15.2, 2323.5, 5858.1 };

Mud_Rect getWindowRect() {
    return windowRect;
}

static MudTestBackend_Options backendOptions = {
    .getWindowRect = getWindowRect
};


int main(void) {
    Mud_App app;
    check(Mud_init(&app, &backendOptions) == MUD_CONTINUE);

    Mud_Rect testWindowRect = Mud_getWindowRect();
    check(memcmp(&testWindowRect, &windowRect, sizeof(Mud_Rect)) == 0);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    return 0;
}
