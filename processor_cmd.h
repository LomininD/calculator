#ifndef PROCESSOR_CMD_H
#define PROCESSOR_CMD_H

#include "processor_properties.h"
#include "stack/stack.h"


const char* decode_cmd (proc_commands cmd);
err_t proc_push(st_t* st, int* code, size_t* ip);
err_t proc_calc(st_t* st, proc_commands cmd);
err_t proc_out(st_t* st);
err_t proc_jmp(st_t* st, int* code, int prg_size, size_t* ip);
err_t proc_cond_jmp(st_t* st, int* code, int prg_size, size_t* ip, proc_commands cmd);
err_t proc_hlt(st_t* st);

#endif
