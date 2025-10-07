#ifndef PROCESSOR_PROPERTIES_H
#define PROCESSOR_PROPERTIES_H

#include "stack/stack.h"
#include "stack/stack_dump.h"

// if file is updated UPDATE VERSION

const int version = 2;

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
    JMP,
    HLT
};

enum err_t
{
    ok,
    error,
    wrong_number,
    wrong_cmd,
    stack_error
};

#endif
