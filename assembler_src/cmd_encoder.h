# ifndef CMD_ENCODER_H
# define CMD_ENCODER_H

#include "../processor_src/processor_properties.h"
#include "assembler_properties.h"

err_t encode_push(files_info* files, assembler_info* asm_data, debug_info* debug);
err_t encode_reg_cmd(files_info* files, assembler_info* asm_data, debug_info* debug, proc_commands cmd);
err_t encode_calc_cmd(files_info* files, assembler_info* asm_data, debug_info* debug, proc_commands cmd);
err_t encode_in_out(files_info* files, assembler_info* asm_data, debug_info* debug, proc_commands cmd);
err_t encode_jmp_cmd(files_info* files, assembler_info* asm_data, debug_info* debug, proc_commands cmd);
err_t encode_hlt(files_info* files, assembler_info* asm_data);

#endif
