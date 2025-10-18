#include "assembler.h"
#include "code_reader.h"
#include "debug.h"
#include "string.h"

// TODO: find place for this array

cmd_struct possible_cmd[] = {{"PUSH", PUSH, number},
                             {"PUSHREG", PUSHREG, string},
                             {"POPREG", POPREG, string},
                             {"ADD", ADD, none},
                             {"SUB", SUB, none},
                             {"DIV", DIV, none},
                             {"MULT", MULT, none},
                             {"SQRT", SQRT, none},
                             {"IN", IN, none},
                             {"OUT", OUT, none},
                             {"JMP", JMP, number},
                             {"JB", JB, number},
                             {"JBE", JBE, number},
                             {"JA", JA, number},
                             {"JAE", JAE, number},
                             {"JE", JE, number},
                             {"JNE", JNE, number},
                             {"HLT", HLT, none}};


// refactor (after)
err_t parse_args(int argc, char* argv[], files_info* files, assembler_info* asm_data)
{
    assert(argv != NULL);
    assert(files != NULL);
    assert(asm_data != NULL);

    if (argc < 2)
    {
        printf("required files were not provided\n");
        return error;
    }

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
                else if (argv[i][ind] == 'h')
                {
                    launch_help();
                    return help;
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
        files->output_file_name = generate_output_name(files->input_file_name);

    files->output_file = fopen(files->output_file_name, "w");

    if (files->input_file == NULL || files->output_file == NULL)
    {
        printf(MAKE_BOLD_RED("ERROR:") " a problem with opening files occurred\n");
        printf(MAKE_GREY("Note: file name may be incorrect\n"));
        return error;
    }

    return ok;
}

char* generate_output_name(char* input_file_name)
{
    assert(input_file_name != NULL);

    const char* out_ext = "out";

    char* output_file_name = (char*) calloc(strlen(input_file_name) + 1, sizeof(char));

    int i = 0;
    while(*(input_file_name + i) != '.')
    {
        *(output_file_name + i) = *(input_file_name + i);
        i++;
    }

    *(output_file_name + i) = *(input_file_name + i);
    i++;

    int j = 0;
    while(*(out_ext + j) != '\0')
    {
        *(output_file_name + i) = *(out_ext + j);
        i++;
        j++;
    }
    puts(input_file_name);
    puts(output_file_name);
    return output_file_name;
}

void launch_help(void)
{
    printf("-d: debug mode\n");
    printf("-o: add output file name after flags\n");
}

// refactor function
err_t asm_ctor(assembler_info* asm_data, debug_info* debug)
{
    assert(asm_data != NULL);
    assert(debug != NULL);

    db_mode debug_mode = asm_data->debug_mode;

    printf_log_msg(debug_mode, "asm_ctor: began initialising\n");

    asm_data->len = 50;
    asm_data->str = (char*) calloc(asm_data->len + 1, sizeof(char));
    asm_data->cmd = UNKNOWN;
    asm_data->end = false;
    asm_data->pos = 0;

    for(int i = 0; i < max_labels_number; i++)
        asm_data->labels[i] = -1;

    debug->current_line = 0;
    debug->got_hlt = false;
    debug->got_out = false;
    debug->not_empty = false;

    printf_log_msg(debug_mode, "asm_ctor: finished initialising\n");
    return ok;
}


err_t fill_file_preamble(assembler_info* asm_data)
{
    assert(asm_data != NULL);

    asm_data->code[0] = 'L';
    asm_data->code[1] = 'M';
    asm_data->code[2] = 'D';
    asm_data->code[3] = version;
    asm_data->code[4] = asm_data->pos;

    // if changed update preamble_size in assembler_properties.h

    return ok;
}

void reset_data(assembler_info* asm_data, int* current_line, FILE* input_file)
{
    assert(asm_data != NULL);
    assert(current_line != NULL);
    assert(input_file != NULL);

    asm_data->pos = 0;
    *current_line = 0;
    asm_data->end = 0;
    rewind(input_file);
}

err_t process_code(files_info* files, assembler_info* asm_data, debug_info* debug)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    db_mode debug_mode = asm_data->debug_mode;

    printf_log_msg(debug_mode, "\n");

    while (!asm_data->end)
    {
        debug->current_line++;
        readline(asm_data, files->input_file);
        if (asm_data->end)
            break;

        printf_log_msg(debug_mode, "current pos in byte_code = %d\n", asm_data->pos);
        printf_log_msg(debug_mode, "assembler: got str: %s\n", asm_data->str);

        int scanned = sscanf(asm_data->str, "%31s", asm_data->raw_cmd);
        if (scanned == -1)
        {
            printf_log_msg(debug_mode, "\n");
            continue;
        }

        printf_log_msg(debug_mode, "assembler: got instruction: %s\n", asm_data->raw_cmd);

        err_t recognized = determine_cmd(files->input_file_name, asm_data, debug->current_line);
        if (recognized != ok)
            return error;

        code_check(debug, asm_data->cmd);

        if (check_prg_size(asm_data->pos, debug_mode) != ok)
            return error;

        printf_log_msg(debug_mode, "\n");

        if (debug_mode == on)
            getchar();
    }
    return ok;
}


err_t check_prg_size(int pos, db_mode debug_mode)
{
    if (pos == max_byte_code_len)
    {
        printf_err(debug_mode, "check_prg_size: max byte code size exceeded\n");
        return error;
    }
    return ok;
}


void code_check(debug_info* debug, proc_commands cmd)
{
    assert(debug != NULL);

    debug->not_empty = true;

    if (cmd == HLT)
        debug->got_hlt = true;

    if (cmd == OUT)
        debug->got_out = true;

}


void output_labels(int* labels, db_mode debug_mode)
{
    assert(labels != NULL);

    printf_log_msg(debug_mode, "\n");

    printf_log_msg(debug_mode, "labels data:\n");

    for (int i = 0; i < max_labels_number; i++)
        printf_log_msg(debug_mode, "%d: %d\n", i, labels[i]);

    printf_log_msg(debug_mode, "\n");
}


err_t determine_cmd(char* file_name, assembler_info* asm_data, int current_line)
{
    assert(file_name != NULL);
    assert(asm_data != NULL);

    db_mode debug_mode = asm_data->debug_mode;

    for(int i = 0; i < _count; i++)
    {
        if (strcmp(possible_cmd[i].name, asm_data->raw_cmd) == 0)
        {
            printf_log_msg(debug_mode, "determine_cmd: recognized %s\n", possible_cmd[i].name);

            asm_data->cmd = possible_cmd[i].cmd_code;
            asm_data->code[asm_data->pos + preamble_size] = possible_cmd[i].cmd_code;
            asm_data->pos++;

            err_t is_read = read_arg(file_name, asm_data, current_line, possible_cmd[i].arg_type);
            return is_read;
        }
    }

    if (asm_data->raw_cmd[0] == ':')
    {
        printf_log_msg(debug_mode, "determine_cmd: recognized label\n");
        return read_label(file_name, asm_data, current_line);
    }

    printf_log_msg(debug_mode, "\n");
    printf_err(debug_mode, "[%s:%d] -> determine_cmd: invalid command (%s)\n", file_name, current_line, asm_data->raw_cmd);
    asm_data->cmd = UNKNOWN;
    return error;
}


void check_warnings(debug_info* debug, char* input_file_name, db_mode debug_mode)
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


void output_code(FILE* fp, int* code, int pos, db_mode debug_mode)
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
