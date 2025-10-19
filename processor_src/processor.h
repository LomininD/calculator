#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "stack/stack.h" // check if all of them are neccessary
#include "stack/font_styles.h"
//#include "stack/stack_dump.h"
#include "../shared/processor_properties.h"
#include "processor_cmd.h"

err_t proc_ctor(proc_info* proc);
err_t proc_dtor(proc_info* proc);
err_t execute_cmd(proc_info* proc, proc_commands cmd);
err_t read_byte_code(proc_info* proc);
//err_t check_ip(proc_info* proc);

#define END_PROCESS                                     \
do{                                                     \
    proc_dtor(proc);                                \
    printf("main: terminating process due to error\n"); \
    return 0;                                           \
}                                                       \
while(0)

#endif
