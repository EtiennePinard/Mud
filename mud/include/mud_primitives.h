#ifndef AE674CE4_A494_4D76_ABB9_440FAE4EE09F
#define AE674CE4_A494_4D76_ABB9_440FAE4EE09F

#include "mud_types.h"

/**
 * @brief An enum representing the primitive render types in Mud
 *
 */
typedef enum Mud_PrimitiveType {
    MUD_PRIMITIVE_BORDER_QUAD,
    MUD_PRIMITIVE_FILLED_QUAD,
    MUD_PRIMITIVE_TEXTURED_QUAD,
    MUD_PRIMITIVE_TEXT
} Mud_PrimitiveType;

/**
 * @brief A struct to hold all information to render a border quad.
 *
 */
typedef struct Mud_BorderQuad {
    Mud_PrimitiveType type;
    Mud_Rect renderRect;
    Mud_Color color;

    float borderThickness;
} Mud_BorderQuad;

/**
 * @brief A struct to hold all information to render a filled quad.
 *
 */
typedef struct Mud_FilledQuad {
    Mud_PrimitiveType type;
    Mud_Rect renderRect;
    Mud_Color color;
} Mud_FilledQuad;

/**
 * @brief A struct to hold all information to render a textured quad.
 *
 */
typedef struct Mud_TextureQuad {
    Mud_PrimitiveType type;
    Mud_Rect renderRect;

    void* textureData; // Backend defined texture data
} Mud_TextureQuad;

/**
 * @brief A struct to hold all information to render text.
 * The fontData format is defined by the backend
 *
 */
typedef struct Mud_Text {
    Mud_PrimitiveType type;
    Mud_Rect renderRect;
    Mud_Color color;

    const char* text;
    void* fontData; // Font data is defined by the backend
} Mud_Text;

/**
 * @brief A struct representing a primitive render object in Mud.
 *
 */
typedef union Mud_Primitive {
    Mud_PrimitiveType type;
    Mud_BorderQuad borderQuad;
    Mud_FilledQuad filledQuad;
    Mud_TextureQuad textureQuad;
    Mud_Text text;
} Mud_Primitive;

#endif /* AE674CE4_A494_4D76_ABB9_440FAE4EE09F */
