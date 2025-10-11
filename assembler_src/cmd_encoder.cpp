#include "cmd_encoder.h"
#include "code_reader.h"


err_t encode_push(files_info* files, assembler_info* asm_data, debug_info* debug)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    err_t is_read = ok;

    if (asm_data->mode == writing_on)
    {
        fprintf(files->output_file, "%d\n", PUSH);
        asm_data->cmd = PUSH;
        is_read = read_number_arg(files, asm_data, debug);
    }

    asm_data->pos += 2;

    return is_read;
}


err_t encode_reg_cmd(files_info* files, assembler_info* asm_data, debug_info* debug, proc_commands cmd)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    err_t is_read = ok;

    if (asm_data->mode == writing_on)
    {
        fprintf(files->output_file, "%d\n", cmd);
        asm_data->cmd = cmd;
        is_read = read_string_arg(files, asm_data, debug);
    }

    asm_data->pos += 2;

    return is_read;
}


err_t encode_calc_cmd(files_info* files, assembler_info* asm_data, debug_info* debug, proc_commands cmd)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    if (asm_data->mode == writing_on)
    {
        fprintf(files->output_file, "%d\n", cmd);
        asm_data->cmd = cmd;
    }

    asm_data->pos += 1;

    return ok;
}


err_t encode_in_out(files_info* files, assembler_info* asm_data, debug_info* debug, proc_commands cmd)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    if (asm_data->mode == writing_on)
    {
        fprintf(files->output_file, "%d\n", cmd);
        asm_data->cmd = cmd;
    }

    asm_data->pos += 1;

    return ok;
}


err_t encode_jmp_cmd(files_info* files, assembler_info* asm_data, debug_info* debug, proc_commands cmd)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    err_t is_read = ok;

    if (asm_data->mode == writing_on)
    {
        fprintf(files->output_file, "%d\n", cmd);
        asm_data->cmd = cmd;
        is_read = read_number_arg(files, asm_data, debug);
    }

    asm_data->pos += 2;

    return is_read;
}


err_t encode_hlt(files_info* files, assembler_info* asm_data)
{
    assert(files != NULL);
    assert(asm_data != NULL);

    if (asm_data->mode == writing_on)
    {
        fprintf(files->output_file, "%d\n", HLT);
        asm_data->cmd = HLT;
    }

    asm_data->pos += 1;

    return ok;
}


