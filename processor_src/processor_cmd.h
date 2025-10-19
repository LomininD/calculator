#ifndef PROCESSOR_CMD_H
#define PROCESSOR_CMD_H

#include "../shared/processor_properties.h"
#include "stack/stack.h"

err_t proc_push(proc_info* proc);
err_t proc_pushreg(proc_info* proc);
err_t proc_popreg(proc_info* proc);
err_t proc_calc(proc_info* proc, proc_commands cmd);
err_t proc_in(proc_info* proc);
err_t get_number(int* number);
err_t proc_out(proc_info* proc);
err_t proc_jmp(proc_info* proc);
err_t proc_cond_jmp(proc_info* proc, proc_commands cmd);
bool check_condition(proc_commands cmd, int a, int b);
const char* decode_cmd (proc_commands cmd);

#endif
