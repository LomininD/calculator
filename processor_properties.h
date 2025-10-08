#ifndef PROCESSOR_PROPERTIES_H
#define PROCESSOR_PROPERTIES_H

#include "stack/stack.h"
#include "stack/stack_dump.h"

// if file is updated UPDATE VERSION

const int version = 6;
const int register_amount = 8;

struct proc_info
{
    int code[100]; // refactor using calloc and realloc or store file size in file
    size_t prg_size;
    st_t st;
    int registers[register_amount];
    size_t ip;
};

enum proc_commands
{
    UNKNOWN,
    PUSH,
    PUSHREG,
    POPREG,
    ADD,
    SUB,
    DIV,
    MULT,
    SQRT,
    IN,
    OUT,
    JMP,
    JB,
    JBE,
    JA,
    JAE,
    JE,
    JNE,
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
