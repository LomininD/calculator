#ifndef INPUT_H
#define INPUT_H

#include "stack/stack.h"
#include "stack/stack_dump.h"


enum calc_commands
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
};

// err_t read_number(int* number);
// void clear_buffer(void);

#endif
