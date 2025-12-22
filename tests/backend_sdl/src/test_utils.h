#ifndef B8BD943C_17F6_466A_8F70_0A1FF81D0342
#define B8BD943C_17F6_466A_8F70_0A1FF81D0342

#include <stdlib.h>
#include <stdio.h> 

#define check(condition) \
    if (!(condition)) { \
        fprintf(stderr, "Condition \"" #condition "\" is false\n"); \
        fprintf(stderr, "See %s in %s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    } \


#endif /* B8BD943C_17F6_466A_8F70_0A1FF81D0342 */
