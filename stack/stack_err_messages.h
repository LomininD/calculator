#ifndef STACK_ERR_MESSAGES_H
#define STACK_ERR_MESSAGES_H

#include <stdio.h>
#include "stack_properties.h"


#define CTOR_ASSERT_ST(ST) do {                                                     \
    if (ST == NULL)                                                                 \
        {                                                                           \
            printf(MAKE_BOLD_RED("ERROR: ") "[from st_ctor] -> no access to stack " \
                                                        "(got NULL pointer)\n");    \
            return no_stack;                                                        \
        }                                                                           \
    } while(0)


#define CTOR_ASSERT_CAPACITY(CAP) do{                                                 \
    if(CAP == 0)                                                                      \
            printf(MAKE_BOLD_RED("WARNING: ") "stack of 0 elements is not usable\n"); \
    } while(0)


#define CTOR_ASSERT_DATA(DATA) do{                                                  \
    if (DATA == NULL)                                                               \
        {                                                                           \
            printf(MAKE_BOLD_RED("ERROR: ") "[from st_ctor] -> could not allocate " \
                                                "memory for stack data\n");         \
            return no_memory;                                                       \
        }                                                                           \
    } while(0)


// #define PUSH_ASSERT_SIZE(ST) do{ \
//     if (ST->size == ST->capacity) \
//         { \
//             printf(MAKE_BOLD_RED("ERROR: ") "[from st_push] -> push failed, stack full\n"); \
//             st_dump(ST); \
//             return stack_full; \
//         } \
//     } while(0)


#define POP_ASSERT_ELEMENTS(ST) do{                                            \
    if (ST->size == 0)                                                         \
        {                                                                      \
            printf(MAKE_BOLD_RED("ERROR: ") "[from st_pop] -> pop failed, "    \
                                                    "no elements in stack\n"); \
            st_dump(ST);                                                       \
            return no_elements;                                                \
        }                                                                      \
    } while(0)

#define ST_EXTEND_ASSERT_DATA(DATA) do{                                             \
    if (DATA == NULL)                                                               \
        {                                                                           \
            printf(MAKE_BOLD_RED("ERROR: ") "[from st_extend from st_push] -> "     \
                "could not reallocate memory for stack data (got NULL pointer)\n"); \
                return no_memory;                                                   \
        }                                                                           \
    } while(0)


#define PUSH_ASSERT_REALLOC(ST) do{        \
        if (ST->size == ST->capacity)      \
        {                                  \
        st_return_err err = st_extend(ST); \
        if (err != no_error)               \
            return err;                    \
        }                                  \
    }while(0)


#define CHECK_STACK(ST) if (!st_verify(st)) return verification_error

#endif
