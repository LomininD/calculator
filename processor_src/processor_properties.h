#ifndef PROCESSOR_PROPERTIES_H
#define PROCESSOR_PROPERTIES_H

#include "stack/stack.h"
#include "stack/stack_dump.h"

// if file is updated UPDATE VERSION

const int version = 7;
const int max_byte_code_len = 1000;
const int register_amount = 8;

struct proc_info
{
    int code[max_byte_code_len]; // refactor using calloc and realloc or store file size in file
    size_t prg_size;
    st_t st;
    int registers[register_amount];
    size_t ip;
};

enum proc_commands
{
    UNKNOWN = -1,
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
    HLT,

    _count
};

enum err_t
{
    ok,
    error,
    help
};

#endif
