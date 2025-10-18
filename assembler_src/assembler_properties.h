# ifndef ASSEMBLER_PROPERTIES_H
# define ASSEMBLER_PROPERTIES_H

#include <stdio.h>
#include "../processor_src/processor_properties.h"
#include "debug.h"

const int max_labels_number = 10;
const int preamble_size = 5;

enum writing_mode
{
    writing_off,
    writing_on
};

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
    int labels[max_labels_number];
    int pos;
    bool end;
    int code[max_byte_code_len]; // do it as a dynamic array (after)
    proc_commands cmd;
    db_mode debug_mode;
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
