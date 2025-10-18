#include "cmd_encoder.h"
#include "code_reader.h"

// think about refactoring

err_t encode_push(char* file_name, assembler_info* asm_data, int current_line)
{
    assert(file_name != NULL);
    assert(asm_data != NULL);

    asm_data->code[asm_data->pos + preamble_size] = PUSH;
    asm_data->pos++;

    err_t is_read = read_number_arg(file_name, asm_data, current_line);

    return is_read;
}


err_t encode_reg_cmd(char* file_name, assembler_info* asm_data, int current_line)
{
    assert(file_name != NULL);
    assert(asm_data != NULL);

    asm_data->code[asm_data->pos + preamble_size] = asm_data->cmd;
    asm_data->pos++;
    err_t is_read = read_string_arg(file_name, asm_data, current_line);

    return is_read;
}


// ask about unused arguments
err_t encode_calc_cmd(char* file_name, assembler_info* asm_data, int current_line)
{
    assert(file_name != NULL);
    assert(asm_data != NULL);

    asm_data->code[asm_data->pos + preamble_size] = asm_data->cmd;
    asm_data->pos++;

    return ok;
}


err_t encode_in_out(char* file_name, assembler_info* asm_data, int current_line)
{
    assert(file_name != NULL);
    assert(asm_data != NULL);

    asm_data->code[asm_data->pos + preamble_size] = asm_data->cmd;
    asm_data->pos++;

    return ok;
}


err_t encode_jmp_cmd(char* file_name, assembler_info* asm_data, int current_line)
{
    assert(file_name != NULL);
    assert(asm_data != NULL);

    asm_data->code[asm_data->pos + preamble_size] = asm_data->cmd;
    asm_data->pos++;

    err_t is_read = read_number_arg(file_name, asm_data, current_line);

    return is_read;
}


err_t encode_hlt(char* file_name, assembler_info* asm_data, int current_line)
{
    assert(file_name != NULL);
    assert(asm_data != NULL);

    asm_data->code[asm_data->pos + preamble_size] = HLT;
    asm_data->pos++;

    return ok;
}


