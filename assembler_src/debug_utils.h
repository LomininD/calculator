# ifndef DEBUG_UTILS_H
# define DEBUG_UTILS_H

#include "assembler_properties.h"

void code_check(debug_info* debug, proc_commands cmd);
err_t check_prg_size(int pos, db_mode debug_mode);
void check_warnings(debug_info* debug, char* input_file_name, db_mode debug_mode);

void output_labels(int* labels, db_mode debug_mode);
void output_code(FILE* fp, int* code, int pos, db_mode debug_mode);

#endif
