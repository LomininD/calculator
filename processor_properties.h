#ifndef PROCESSOR_PROPERTIES_H
#define PROCESSOR_PROPERTIES_H

#include "stack/stack.h"
#include "stack/stack_dump.h"


enum proc_commands
{
    UNKNOWN,
    PUSH,
    ADD,
    SUB,
    DIV,
    MULT,
    SQRT,
    OUT,
    HLT
};

enum err_t
{
    ok,
    wrong_number,
    wrong_cmd,
    stack_error
};

#endif
