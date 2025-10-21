#ifndef PROCESSOR_PROPERTIES_H
#define PROCESSOR_PROPERTIES_H

#include "../processor_src/stack/stack.h"
#include "../processor_src/stack/stack_dump.h"
#include "debug.h"

// if file is updated UPDATE VERSION

const int version = 10;
const int max_byte_code_len = 1000;
const int register_amount = 8;
const int accuracy = 1000;

struct proc_modes_type
{
    md_t debug_mode;
    md_t float_mode;
};


struct proc_info
{
    proc_modes_type proc_modes; // ?
    FILE* byte_code_file; // ?
    int* code;
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
    DMP,
    HLT,

    _cmd_count
};

enum err_t
{
    error,
    ok,
    help
};

enum arg_t
{
    none,
    number,
    string,
    label
};

struct cmd_struct
{
    char name[10];
    proc_commands cmd_code;
    arg_t arg_type;
};

extern cmd_struct possible_cmd[];

#endif
