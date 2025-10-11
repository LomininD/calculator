# ifndef CODE_READER_H
# define CODE_READER_H

#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#include "../processor_src/processor_properties.h"
#include "../processor_src/stack/font_styles.h"
#include "assembler_properties.h"


err_t read_number_arg(files_info* files, assembler_info* asm_data, debug_info* debug);
err_t read_string_arg(files_info* files, assembler_info* asm_data, debug_info* debug);
err_t read_label(files_info* files, assembler_info* asm_data, debug_info* debug);
err_t replace_label(files_info* files, assembler_info* asm_data, debug_info* debug);
int decode_reg_name(char* arg);
void readline(assembler_info* asm_data, files_info* file);


#endif
