# ifndef CMD_STRUCTURES_H
# define CMD_STRUCTURES_H

#include "../processor_src/processor_properties.h"
#include "assembler_properties.h"

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
    err_t (*func_ptr)(files_info* , assembler_info*, debug_info*, proc_commands);
    arg_t arg_type;
};

#endif
