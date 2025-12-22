#ifndef A17649AB_016F_47B3_A218_BD971032F645
#define A17649AB_016F_47B3_A218_BD971032F645

// Function annotation for public api functions
#ifdef _WIN32
#define MUD_API __declspec(dllexport)
#else
#define MUD_API
#endif

#include <stdint.h>
#include <stdbool.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

/**
 * @brief A struct to represent an RGBA color.
 * 
 */
typedef struct Mud_Color {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} Mud_Color;

/**
 * @brief An enum which tells if the app should 
 * continue or terminate with failure or success.
 * 
 */
typedef enum Mud_AppResult {
    MUD_CONTINUE,
    MUD_TERMINATE_WITH_SUCCESS,
    MUD_TERMINATE_WITH_FAILURE
} Mud_AppResult;

/**
 * @brief A struct representing a rectangle with a 
 * x position, y position, width and height.
 * 
 */
typedef struct Mud_Rect {
    float x;
    float y;
    float width;
    float height;
} Mud_Rect;

/**
 * @brief A struct representing a point with a 
 * x position and y position.
 * 
 */
typedef struct Mud_Point {
    float x;
    float y;
} Mud_Point;

/**
 * @brief Checks if a point is inside a rectangle
 * 
 * @param point The point to check if inside the rect
 * @param rect The rectangle to check if it contains the point
 * @return bool true if inside the rectangle false otherwise
 */
static inline bool Mud_pointInRect(Mud_Point point, Mud_Rect rect) {
    return
        rect.x <= point.x && point.x < rect.x + rect.width &&
        rect.y <= point.y && point.y < rect.y + rect.height;
}


#endif /* A17649AB_016F_47B3_A218_BD971032F645 */
