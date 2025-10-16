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
err_t initialise_assembler(int argc, char* argv[], assembler_info* asm_data, debug_info* debug);
err_t fill_file_preamble(files_info* files, int pos);
err_t process_code(files_info* files, assembler_info* asm_data, debug_info* debug);
err_t preliminary_process_code(files_info* files, assembler_info* asm_data, debug_info* debug);
void output_labels(assembler_info* asm_data);
err_t determine_cmd(files_info* files, assembler_info* asm_data, debug_info* debug);
void check_warnings(debug_info* debug, files_info* files, assembler_info* asm_data);


#endif
