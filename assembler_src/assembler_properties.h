# ifndef ASSEMBLER_PROPERTIES_H
# define ASSEMBLER_PROPERTIES_H

#include <stdio.h>
#include "../processor_src/processor_properties.h"

// extern const char* output_name =  "program.out";

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
    int pos;
};

struct debug_info
{
    int current_line;
    bool got_hlt;
    bool got_out;
    bool not_empty;
};

#define $$ fprintf(stderr, "line: %d\n", __LINE__);

#define $$$(...) { fprintf(stderr, "%s... ", #__VA_ARGS__); __VA_ARGS__; fprintf(stderr, "done\n"); }

#endif
