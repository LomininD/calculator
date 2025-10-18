# ifndef ASSEMBLER_H
# define ASSEMBLER_H

#include <string.h>
#include "../processor_src/processor_properties.h"
#include "../processor_src/stack/font_styles.h"
#include "assembler_properties.h"
#include "debug_utils.h"

// if no argument for output is given, output to file with the same name but .out extention
// solve problem with unexpected symbols
// debug messages in console (in debug mode) - user can redirect them to file
// debug messages to log file (in debug mode)
// errors and introductory information in console (always)

err_t asm_ctor(assembler_info* asm_data, debug_info* debug);
err_t fill_file_preamble(assembler_info* asm_data);
err_t process_code(files_info* files, assembler_info* asm_data, debug_info* debug);
void reset_data(assembler_info* asm_data, int* current_line, FILE* input_file);
err_t determine_cmd(char* file_name, assembler_info* asm_data, int current_line);


#endif
