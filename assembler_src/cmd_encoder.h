# ifndef CMD_ENCODER_H
# define CMD_ENCODER_H

#include "../processor_src/processor_properties.h"
#include "assembler_properties.h"

//err_t encode_push(char* file_name, assembler_info* asm_data, int current_line);
err_t encode_reg_cmd(char* file_name, assembler_info* asm_data, int current_line);
err_t encode_calc_cmd(char* file_name, assembler_info* asm_data, int current_line);
err_t encode_in_out(char* file_name, assembler_info* asm_data, int current_line);
err_t encode_jmp_cmd(char* file_name, assembler_info* asm_data, int current_line);
err_t encode_hlt(char* file_name, assembler_info* asm_data, int current_line);

#endif
