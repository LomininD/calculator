#include "cmd_encoder.h"
#include "code_reader.h"


err_t encode_push(files_info* files, assembler_info* asm_data, debug_info* debug)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    fprintf(files->output_file, "%d\n", PUSH);
    asm_data->cmd = PUSH;

    int number = 0;
    err_t is_read = read_number_arg(files, asm_data, debug, &number);

    return is_read;
}


err_t encode_reg_cmd(files_info* files, assembler_info* asm_data, debug_info* debug, proc_commands cmd)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    fprintf(files->output_file, "%d\n", cmd);
    asm_data->cmd = cmd;

    int number = 0;
    err_t is_read = read_string_arg(files, asm_data, debug, &number);

    return is_read;
}


err_t encode_calc_cmd(files_info* files, assembler_info* asm_data, debug_info* debug, proc_commands cmd)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    fprintf(files->output_file, "%d\n", cmd);
    asm_data->cmd = cmd;

    return ok;
}


err_t encode_in_out(files_info* files, assembler_info* asm_data, debug_info* debug, proc_commands cmd)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    fprintf(files->output_file, "%d\n", cmd);
    asm_data->cmd = cmd;
    return ok;
}


err_t encode_jmp_cmd(files_info* files, assembler_info* asm_data, debug_info* debug, proc_commands cmd)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    fprintf(files->output_file, "%d\n", cmd);
    asm_data->cmd = cmd;

    int number = 0;
    err_t is_read = read_number_arg(files, asm_data, debug, &number);

    return is_read;
}


err_t encode_hlt(files_info* files, assembler_info* asm_data)
{
    assert(files != NULL);
    assert(asm_data != NULL);

    fprintf(files->output_file, "%d\n", HLT);
    asm_data->cmd = HLT;
    return ok;
}


