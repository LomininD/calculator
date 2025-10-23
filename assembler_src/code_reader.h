# ifndef CODE_READER_H
# define CODE_READER_H

#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#include "../shared/processor_properties.h"
#include "../processor_src/stack/font_styles.h"
#include "../shared/assembler_properties.h"

err_t read_arg(const char* file_name, assembler_info* asm_data, int current_line, arg_t arg_type);
err_t read_number_arg(const char* file_name, assembler_info* asm_data, int current_line);
err_t read_string_arg(const char* file_name, assembler_info* asm_data, int current_line);
err_t read_index(const char* file_name, assembler_info* asm_data, int current_line);
err_t read_label(const char* file_name, assembler_info* asm_data, int current_line);
err_t replace_label(const char* file_name, assembler_info* asm_data, int current_line);
int decode_reg_name(const char* arg);
void readline(assembler_info* asm_data, FILE* fp);


#endif
