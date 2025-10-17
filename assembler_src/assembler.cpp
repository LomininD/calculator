#include "assembler.h"
#include "code_reader.h"
#include "cmd_encoder.h"
#include "debug.h"

const char* output_name =  "program.out"; // change

err_t parse_args(int argc, char* argv[], files_info* files, assembler_info* asm_data) // add help
{
    assert(argv != NULL);
    assert(files != NULL);

    if (argc < 2)
    {
        printf("required files were not provided\n");
        return error;
    }

    files->output_file_name = (char*) output_name; // change
    files->input_file_name = argv[1];
    files->input_file = fopen(files->input_file_name, "r");

    bool file_determined = false;

    for (int i = 2; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            int ind = 1;
            while(argv[i][ind] != '\0')
            {
                if (argv[i][ind] == 'd')
                {
                    asm_data->debug_mode = on;
                }
                else if (argv[i][ind] == 'o')
                {
                    file_determined = true;
                }
                else
                {
                    printf(MAKE_BOLD_RED("ERROR:") " wrong command line argument\n");
                    return error;
                }
                ind++;
            }
        }
        else
        {
            if (file_determined)
            {
                files->output_file_name = argv[i];
            }
            else
            {
                printf(MAKE_BOLD_RED("ERROR:") " wrong command line argument\n");
                return error;
            }
        }
    }

    if (!file_determined)
        files->output_file_name = (char*) output_name; // change

    files->output_file = fopen(files->output_file_name, "w");

    if (files->input_file == NULL || files->output_file == NULL)
    {
        printf(MAKE_BOLD_RED("ERROR:") " a problem with opening files occurred\n");
        printf(MAKE_GREY("Note: file name may be incorrect\n"));
        return error;
    }

    return ok;
}

// refactor function
err_t initialise_assembler(int argc, char* argv[], assembler_info* asm_data, debug_info* debug)
{
    assert(argv != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    db_mode debug_mode = asm_data->debug_mode;

    printf_log_msg(debug_mode, "initialise_assembler: began initialising\n", NULL);

    asm_data->len = 50;
    asm_data->str = (char*) calloc(asm_data->len + 1, sizeof(char));
    asm_data->cmd = UNKNOWN;
    asm_data->end = false;
    asm_data->pos = 0;
    asm_data->w_mode = writing_off;

    for(int i = 0; i < max_labels_number; i++)
        asm_data->labels[i] = -1;

    debug->current_line = 0;
    debug->got_hlt = false;
    debug->got_out = false;
    debug->not_empty = false;

    printf_log_msg(debug_mode, "initialise_assembler: finished initialising\n", NULL);
    return ok;
}


err_t fill_file_preamble(files_info* files, int pos)
{
    assert(files != NULL);

    fprintf(files->output_file, "%d\n", 'L');
    fprintf(files->output_file, "%d\n", 'M');
    fprintf(files->output_file, "%d\n", 'D');

    fprintf(files->output_file, "%d\n", version);

    //fprintf(files->output_file, "%d\n", program_size);

    return ok;
}


err_t process_code(files_info* files, assembler_info* asm_data, debug_info* debug)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    db_mode debug_mode = asm_data->debug_mode;

    asm_data->pos = 0;
    debug->current_line = 0;
    asm_data->end = 0;
    asm_data->w_mode = writing_on;

    while (!asm_data->end)
    {
        debug->current_line++;
        readline(asm_data, files);

        if (asm_data->end)
            break;

        debug->not_empty = true;

        printf_log_msg(debug_mode, "current pos in byte_code = %d\n", asm_data->pos);

        printf_log_msg(debug_mode, "assembler: got str: %s\n", asm_data->str);

        int scanned = sscanf(asm_data->str, "%31s", asm_data->raw_cmd);
        if (scanned == -1)
        {
            printf_log_msg(debug_mode, "\n", NULL);
            continue;
        }

        printf_log_msg(debug_mode, "assembler: got instruction: %s\n", asm_data->raw_cmd);

        err_t recognized = determine_cmd(files, asm_data, debug);

        if (recognized != ok)
            return error;

        printf_log_msg(debug_mode, "\n", NULL);

        if (debug_mode == on)
            getchar();
    }
    return ok;
}

// combine and add buffer

err_t preliminary_process_code(files_info* files, assembler_info* asm_data, debug_info* debug)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    db_mode debug_mode = asm_data->debug_mode;

    printf_log_msg(debug_mode, "\n", NULL);

    asm_data->w_mode = writing_off;

    while (!asm_data->end)
    {
        debug->current_line++;
        readline(asm_data, files);

        if (asm_data->end)
            break;

        int scanned = sscanf(asm_data->str, "%31s", asm_data->raw_cmd);
        if (scanned == -1)
        {
            continue;
        }

        err_t recognized = determine_cmd(files, asm_data, debug);

        if (recognized != ok)
            return error;

        if (asm_data->cmd == HLT)
            debug->got_hlt = true;

        if (asm_data->cmd == OUT)
            debug->got_out = true;
    }

    printf_log_msg(debug_mode, "\n", NULL);
    return ok;
}


void output_labels(assembler_info* asm_data)
{
    assert(asm_data != NULL);

    db_mode debug_mode = asm_data->debug_mode;

    printf_log_msg(debug_mode, "\n", NULL);

    printf_log_msg(debug_mode, "labels data:\n", NULL);

    for (int i = 0; i < max_labels_number; i++)
        printf_log_msg(debug_mode, "%d: %d\n", i, asm_data->labels[i], NULL);

    printf_log_msg(debug_mode, "\n", NULL);
}


err_t determine_cmd(files_info* files, assembler_info* asm_data, debug_info* debug) // ask how to refactor
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    db_mode debug_mode = asm_data->debug_mode;

    if (strcmp("PUSH", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized push\n", NULL);
        return encode_push(files, asm_data, debug);
    }
    else if (strcmp("PUSHREG", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized pushreg\n", NULL);
        return encode_reg_cmd(files, asm_data, debug, PUSHREG);
    }
    else if (strcmp("POPREG", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized popreg\n", NULL);
        return encode_reg_cmd(files, asm_data, debug, POPREG);
    }
    else if (strcmp("ADD", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized add\n", NULL);
        return encode_calc_cmd(files, asm_data, debug, ADD);
    }
    else if (strcmp("SUB", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized sub\n", NULL);
        return encode_calc_cmd(files, asm_data, debug, SUB);
    }
    else if (strcmp("DIV", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized div\n", NULL);
        return encode_calc_cmd(files, asm_data, debug, DIV);
    }
    else if (strcmp("MULT", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized mult\n", NULL);
        return encode_calc_cmd(files, asm_data, debug, MULT);
    }
    else if (strcmp("SQRT", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized sqrt\n", NULL);
        return encode_calc_cmd(files, asm_data, debug, SQRT);
    }
    else if (strcmp("IN", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized in\n", NULL);
        return encode_in_out(files, asm_data, debug, IN);
    }
    else if (strcmp("OUT", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized out\n", NULL);
        return encode_in_out(files, asm_data, debug, OUT);
    }
    else if (strcmp("JMP", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized jmp\n", NULL);
        return encode_jmp_cmd(files, asm_data, debug, JMP);
    }
    else if (strcmp("JB", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized jb\n", NULL);
        return encode_jmp_cmd(files, asm_data, debug, JB);
    }
    else if (strcmp("JBE", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized jbe\n", NULL);
        return encode_jmp_cmd(files, asm_data, debug, JBE);
    }
    else if (strcmp("JA", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized ja\n", NULL);
        return encode_jmp_cmd(files, asm_data, debug, JA);
    }
    else if (strcmp("JAE", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized jae\n", NULL);
        return encode_jmp_cmd(files, asm_data, debug, JAE);
    }
    else if (strcmp("JE", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized je\n", NULL);
        return encode_jmp_cmd(files, asm_data, debug, JE);
    }
    else if (strcmp("JNE", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized jne\n", NULL);
        return encode_jmp_cmd(files, asm_data, debug, JNE);
    }
    else if (asm_data->raw_cmd[0] == ':')
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized label\n", NULL);
        return read_label(files, asm_data, debug);
    }
    else if (strcmp("HLT", asm_data->raw_cmd) == 0)
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized hlt\n", NULL);
        return encode_hlt(files, asm_data);
    }

    printf_log_msg(debug_mode, "\n", NULL);
    printf_err(debug_mode, "[%s:%d] -> determine_cmd: invalid command (%s)\n", files->input_file_name, debug->current_line, asm_data->raw_cmd);
    asm_data->cmd = UNKNOWN;
    return error;
}


void check_warnings(debug_info* debug, files_info* files, assembler_info* asm_data)
{
    assert(debug != NULL);
    assert(files != NULL);

    db_mode debug_mode = asm_data->debug_mode;

    if (debug->not_empty)
    {
        if (!debug->got_out)
        {
            printf_warn(debug_mode, "[%s:%d:] -> assembler: calculations seem to have no effect\n", files->input_file_name, debug->current_line);
            printf_note(debug_mode, "Note: you may forgot OUT instruction in your code\n", NULL);
        }

        if (!debug->got_hlt)
            printf_warn(debug_mode, "[%s:%d:] -> assembler: no HLT instruction in the end of the program\n", files->input_file_name, debug->current_line);
    }
    else
    {
        printf_warn(debug_mode, "assembler: got blank file\n", NULL);
    }
}
