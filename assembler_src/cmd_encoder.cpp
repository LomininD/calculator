#include "cmd_encoder.h"
#include "code_reader.h"


err_t encode_push(files_info* files, assembler_info* asm_data, debug_info* debug)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    asm_data->code[asm_data->pos + preamble_size] = PUSH;
    asm_data->pos++;

    err_t is_read = read_number_arg(files, asm_data, debug);
    asm_data->cmd = PUSH; // put in determine cmd

    return is_read;
}


err_t encode_reg_cmd(files_info* files, assembler_info* asm_data, debug_info* debug, proc_commands cmd)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    asm_data->code[asm_data->pos + preamble_size] = cmd;
    asm_data->pos++;
    err_t is_read = read_string_arg(files, asm_data, debug);

    asm_data->cmd = cmd;

    return is_read;
}


err_t encode_calc_cmd(files_info* files, assembler_info* asm_data, debug_info* debug, proc_commands cmd)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    asm_data->code[asm_data->pos + preamble_size] = cmd;
    asm_data->pos++;

    asm_data->cmd = cmd;

    return ok;
}


err_t encode_in_out(files_info* files, assembler_info* asm_data, debug_info* debug, proc_commands cmd)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);


    asm_data->code[asm_data->pos + preamble_size] = cmd;
    asm_data->pos++;

    asm_data->cmd = cmd;

    return ok;
}


err_t encode_jmp_cmd(files_info* files, assembler_info* asm_data, debug_info* debug, proc_commands cmd)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    asm_data->code[asm_data->pos + preamble_size] = cmd;
    asm_data->pos++;

    err_t is_read = read_number_arg(files, asm_data, debug);

    asm_data->cmd = cmd;

    return is_read;
}


err_t encode_hlt(files_info* files, assembler_info* asm_data)
{
    assert(files != NULL);
    assert(asm_data != NULL);

    asm_data->code[asm_data->pos + preamble_size] = HLT;
    asm_data->pos++;

    asm_data->cmd = HLT;

    return ok;
}


