# ifndef ASSEMBLER_H
# define ASSEMBLER_H

#include <string.h>
#include "../processor_src/processor_properties.h"
#include "../processor_src/stack/font_styles.h"
#include "assembler_properties.h"

// if no argument for output is given, output to file with the same name but .out extention
// solve problem with unexpected symbols
// debug messages in console (in debug mode) - user can redirect them to file
// debug messages to log file (in debug mode)
// errors and introductory information in console (always)

err_t parse_args(int argc, char* argv[], files_info* files, assembler_info* asm_data);
char* generate_output_name(char* input_file_name);
void launch_help(void);
void reset_data(assembler_info* asm_data, int* current_line, FILE* input_file);
err_t asm_ctor(assembler_info* asm_data, debug_info* debug);
err_t fill_file_preamble(assembler_info* asm_data);
err_t process_code(files_info* files, assembler_info* asm_data, debug_info* debug);
void code_check(debug_info* debug, proc_commands cmd);
err_t check_prg_size(int pos, db_mode debug_mode);
void output_labels(int* labels, db_mode debug_mode);
err_t determine_cmd(char* file_name, assembler_info* asm_data, int current_line);
void check_warnings(debug_info* debug, char* input_file_name, db_mode debug_mode);
void output_code(FILE* fp, int* code, int pos, db_mode debug_mode);


#endif
