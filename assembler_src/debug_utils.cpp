#include <assert.h>
#include "debug_utils.h"


void code_check(debug_info* debug, proc_commands cmd)
{
    assert(debug != NULL);

    debug->not_empty = true;

    if (cmd == HLT)
        debug->got_hlt = true;

    if (cmd == OUT)
        debug->got_out = true;

}


err_t check_prg_size(int pos, md_t debug_mode)
{
    if (pos == max_byte_code_len)
    {
        printf_err(debug_mode, "check_prg_size: max byte code size exceeded\n");
        return error;
    }
    return ok;
}


void check_warnings(debug_info* debug, char* input_file_name, md_t debug_mode)
{
    assert(debug != NULL);
    assert(input_file_name != NULL);

    if (debug->not_empty)
    {
        if (!debug->got_out)
        {
            printf_warn(debug_mode, "[%s:%d:] -> assembler: calculations seem to have no effect\n", input_file_name, debug->current_line);
            printf_note(debug_mode, "Note: you may forgot OUT instruction in your code\n", NULL);
        }

        if (!debug->got_hlt)
            printf_warn(debug_mode, "[%s:%d:] -> assembler: no HLT instruction in the end of the program\n", input_file_name, debug->current_line);
    }
    else
    {
        printf_warn(debug_mode, "assembler: got blank file\n", NULL);
    }
}


void output_labels(int* labels, md_t debug_mode)
{
    assert(labels != NULL);

    printf_log_msg(debug_mode, "\n");

    printf_log_msg(debug_mode, "labels data:\n");

    for (int i = 0; i < max_labels_number; i++)
        printf_log_msg(debug_mode, "%d: %d\n", i, labels[i]);

    printf_log_msg(debug_mode, "\n");
}


void output_code(FILE* fp, int* code, int pos, md_t debug_mode)
{
    assert(fp != NULL);
    assert(code != NULL);

    if (debug_mode == on)
    {
        printf_log_msg(debug_mode, "\n");

        for (int i = 0; i < pos + preamble_size; ++i)
        {
            printf_log_msg(debug_mode, "%d ", code[i]);
        }

        printf_log_msg(debug_mode, "\n\n");
    }

    for (int i = 0; i < pos + preamble_size; ++i)
        fprintf(fp, "%d\n", code[i]);
}
