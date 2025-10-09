# ifndef ASSEMBLER_PROPERTIES_H
# define ASSEMBLER_PROPERTIES_H

#include <stdio.h>
#include "../processor_src/processor_properties.h"


struct files_info
{
    char* input_file_name;
    char* output_file_name;
    FILE* input_file;
    FILE* output_file;
};

struct assembler_info
{
    size_t len;
    char* str;
    char raw_cmd[32];
    proc_commands cmd;
    bool end;
};

struct debug_info
{
    int current_line;
    bool got_hlt;
    bool got_out;
    bool not_empty;
};


#endif
