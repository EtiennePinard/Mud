#include "../../../mud/include/mud.h"
#include "../../../backends/sdl_backend/include/sdl_backend.h"

#include "test_utils.h"

static Mud_Rect windowRect = {
    .x = 0,
    .y = 0,
    .width = 100,
    .height = 200
};

int main(void) {
    Mud_App app;
    MudSDLBackend_Options options = {
        .sdlFlags = 0,
        .windowFlags = 0,
        .window_title = "Test window",
        .windowWidth = windowRect.width,
        .windowHeight = windowRect.height,
        .windowX = SDL_WINDOWPOS_CENTERED,
        .windowY = SDL_WINDOWPOS_CENTERED
    };

    check(Mud_init(&app, &options) == MUD_CONTINUE);

    Mud_Rect backendWindowRect = Mud_getWindowRect();
    check(memcmp(&backendWindowRect, &windowRect, sizeof(Mud_Rect)) == 0);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    return 0;
}
