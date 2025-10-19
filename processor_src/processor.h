#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "stack/stack.h"
#include "stack/font_styles.h"
#include "../shared/processor_properties.h"
#include "processor_cmd.h"

err_t proc_ctor(proc_info* proc);
err_t proc_dtor(proc_info* proc);
err_t execute_cmd(proc_info* proc, proc_commands cmd);
err_t read_byte_code(proc_info* proc);
//err_t check_ip(proc_info* proc);

#define END_PROCESS(mode)                               \
do{                                                     \
    printf_abortion(mode, "main\n");                    \
    proc_dtor(proc);                                    \
    return 0;                                           \
}                                                       \
while(0)

#endif
