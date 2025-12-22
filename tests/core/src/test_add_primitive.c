#include "../../../mud/include/mud.h"
#include "test_utils.h"

int main(void) {
    Mud_App app;
    Mud_init(&app, NULL);

    Mud_Primitive primitive;
    primitive.filledQuad = (Mud_FilledQuad) {
        .type = MUD_PRIMITIVE_FILLED_QUAD,
        .renderRect = {0, 0, 100, 50},
        .color = {255, 0, 0, 255}
    };

    Mud_addPrimitive(&app, &primitive);

    check(app.renderQueue.count == 1);
    check(app.renderQueue.commands[0].type == MUD_PRIMITIVE_FILLED_QUAD);

    Mud_terminate(&app, MUD_TERMINATE_WITH_SUCCESS);
    return 0;
}
