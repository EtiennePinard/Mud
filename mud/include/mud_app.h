#ifndef A3F64581_9079_4238_8C78_543A6CB7F6C6
#define A3F64581_9079_4238_8C78_543A6CB7F6C6

#include <stdatomic.h>

/**
 * @brief The Mud_App struct is the struct contains
 * all the information necessary to render the app and handle events.
 *
 * The app layout information is stored into a Scene. A Scene is
 * a struct which contains rendering information about the layout.
 * The rendering information is stored into a list of LayoutBox structs.
 * A LayoutBox has a rectangle associated to it along with a RenderFunction
 * and multiple EventCallbackBox functions. The RenderFunction is responsible to
 * render the LayoutBox based on the  Scene's data and the rectangle associated
 * to the LayoutBox. The EventCallbackBox functions will be called by Mud when
 * their respective events happen.
 *
 * The events local to LayoutBoxes are stored into the Mud_Scene field, the
 * global events and mouse state are stored into the Mud_AppEvents struct.
 */
typedef struct Mud_App Mud_App;

#endif /* A3F64581_9079_4238_8C78_543A6CB7F6C6 */
