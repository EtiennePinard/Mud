#ifndef A9CF2E30_E4AE_4797_BF03_F7A058F5287A
#define A9CF2E30_E4AE_4797_BF03_F7A058F5287A

#include <stdlib.h>
#include <stdio.h> 

#define check(condition) \
    if (!(condition)) { \
        fprintf(stderr, "Condition \"" #condition "\" is false\n"); \
        fprintf(stderr, "See %s in %s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    } \


#endif /* A9CF2E30_E4AE_4797_BF03_F7A058F5287A */
