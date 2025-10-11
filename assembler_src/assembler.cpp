#include "assembler.h"
#include "code_reader.h"
#include "cmd_encoder.h"

const char* output_name =  "program.out";

err_t initialise_assembler(int argc, char* argv[], files_info* files, assembler_info* asm_data, debug_info* debug)
{
    assert(argv != NULL);
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    printf("initialise_assembler: began initialising\n");

    asm_data->len = 50;
    asm_data->str = (char*) calloc(asm_data->len + 1, sizeof(char));
    asm_data->cmd = UNKNOWN;
    asm_data->end = false;
    asm_data->pos = 0;
    asm_data->mode = writing_off;

    for(int i = 0; i < max_labels_number; i++)
        asm_data->labels[i] = -1;

    debug->current_line = 0;
    debug->got_hlt = false;
    debug->got_out = false;
    debug->not_empty = false;

    err_t got_files = get_files_data(argc, argv, files);

    if (got_files != ok)
    {
        printf("initialise_assembler: failed initialisation\n");
        return error;
    }

    printf("initialise_assembler: finished initialising\n");
    return ok;
}


err_t get_files_data(int argc, char* argv[], files_info* files)
{
    assert(argv != NULL);
    assert(files != NULL);

    if (argc < 2)
    {
        printf("get_files_data: " MAKE_BOLD_RED("ERROR:") " required files were not provided\n");
        return error;

    }
    else if (argc > 3)
    {
        printf("get_files_data: " MAKE_BOLD_RED("ERROR:") " got to many files\n");
        return error;
    }

    files->output_file_name = (char*) output_name;

    files->input_file_name = argv[1];
    files->input_file = fopen(files->input_file_name, "r");

    if (argc == 3)
        files->output_file_name = argv[2];

    files->output_file = fopen(files->output_file_name, "w");

    if (files->input_file == NULL || files->output_file == NULL)
    {
        printf("get_files_data: " MAKE_BOLD_RED("ERROR:") " a problem with opening files occurred\n");
        printf(MAKE_GREY("Note: file name may be incorrect\n"));
        return error;
    }

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

    asm_data->pos = 0;
    asm_data->end = 0;
    asm_data->mode = writing_on;

    while (!asm_data->end)
    {
        debug->current_line++;
        readline(asm_data, files);

        if (asm_data->end)
            break;

        debug->not_empty = true;

        printf("current pos in byte_code = %d\n", asm_data->pos);

        printf("assembler: got str: %s\n", asm_data->str);

        int scanned = sscanf(asm_data->str, "%31s", asm_data->raw_cmd);
        if (scanned == -1)
        {
            printf("\n");
            continue;
        }

        printf("assembler: got instruction: %s\n", asm_data->raw_cmd);

        err_t recognized = determine_cmd(files, asm_data, debug);

        if (recognized != ok)
            return error;

        printf("\n");

        if (asm_data->cmd == HLT)
        {
            debug->got_hlt = true;
        }

        if (asm_data->cmd == OUT)
            debug->got_out = true;

        getchar();
    }
    return ok;
}


err_t preliminary_process_code(files_info* files, assembler_info* asm_data, debug_info* debug)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    asm_data->mode = writing_off;

    while (!asm_data->end)
    {
        debug->current_line++;
        readline(asm_data, files);

        if (asm_data->end)
            break;

        int scanned = sscanf(asm_data->str, "%31s", asm_data->raw_cmd);
        if (scanned == -1)
        {
            printf("\n");
            continue;
        }

        err_t recognized = determine_cmd(files, asm_data, debug);

        if (recognized != ok)
            return error;
    }
    return ok;
}


void output_labels(assembler_info* asm_data)
{
    assert(asm_data != NULL);
    printf("labels data:\n");

    for (int i = 0; i < max_labels_number; i++)
        printf("%d: %d\n", i, asm_data->labels[i]);

    printf("\n");
}


err_t determine_cmd(files_info* files, assembler_info* asm_data, debug_info* debug) // refactor this piece of shit
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    if (strcmp("PUSH", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized push\n");
        return encode_push(files, asm_data, debug);
    }
    else if (strcmp("PUSHREG", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized pushreg\n");
        return encode_reg_cmd(files, asm_data, debug, PUSHREG);
    }
    else if (strcmp("POPREG", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized popreg\n");
        return encode_reg_cmd(files, asm_data, debug, POPREG);
    }
    else if (strcmp("ADD", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized add\n");
        return encode_calc_cmd(files, asm_data, debug, ADD);
    }
    else if (strcmp("SUB", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized sub\n");
        return encode_calc_cmd(files, asm_data, debug, SUB);
    }
    else if (strcmp("DIV", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized div\n");
        return encode_calc_cmd(files, asm_data, debug, DIV);
    }
    else if (strcmp("MULT", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized mult\n");
        return encode_calc_cmd(files, asm_data, debug, MULT);
    }
    else if (strcmp("SQRT", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized sqrt\n");
        return encode_calc_cmd(files, asm_data, debug, SQRT);
    }
    else if (strcmp("IN", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized in\n");
        return encode_in_out(files, asm_data, debug, IN);
    }
    else if (strcmp("OUT", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized out\n");
        return encode_in_out(files, asm_data, debug, OUT);
    }
    else if (strcmp("JMP", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized jmp\n");
        return encode_jmp_cmd(files, asm_data, debug, JMP);

    }
    else if (strcmp("JB", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized jb\n");
        return encode_jmp_cmd(files, asm_data, debug, JB);
    }
    else if (strcmp("JBE", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized jbe\n");
        return encode_jmp_cmd(files, asm_data, debug, JBE);
    }
    else if (strcmp("JA", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized ja\n");
        return encode_jmp_cmd(files, asm_data, debug, JA);
    }
    else if (strcmp("JAE", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized jae\n");
        return encode_jmp_cmd(files, asm_data, debug, JAE);
    }
    else if (strcmp("JE", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized je\n");
        return encode_jmp_cmd(files, asm_data, debug, JE);
    }
    else if (strcmp("JNE", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized jne\n");
        return encode_jmp_cmd(files, asm_data, debug, JNE);
    }
    else if (asm_data->raw_cmd[0] == ':')
    {
        printf("determine_cmd: recognized label\n");
        return read_label(files, asm_data, debug);
    }
    else if (strcmp("HLT", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized hlt\n");
        return encode_hlt(files, asm_data);
    }

    printf("\n");
    printf("determine_cmd: %s:%d: " MAKE_BOLD_RED("ERROR:") " invalid command (%s)\n", files->input_file_name, debug->current_line, asm_data->raw_cmd);
    asm_data->cmd = UNKNOWN;
    return error;
}


void check_warnings(debug_info* debug, files_info* files)
{
    assert(debug != NULL);
    assert(files != NULL);

    if (debug->not_empty)
    {
        if (!debug->got_out)
        {
            printf("assembler: %s:%d: " MAKE_BOLD("WARNING:") " calculations seem to have no effect\n", files->input_file_name, debug->current_line);
            printf(MAKE_GREY("Note: you may forgot OUT instruction in your code\n\n"));
        }

        if (!debug->got_hlt)
            printf("assembler: %s:%d: " MAKE_BOLD("WARNING:") " no HLT instruction in the end of the program\n\n", files->input_file_name, debug->current_line);
    }
    else
    {
        printf("assembler: " MAKE_BOLD("WARNING:") " got blank file\n\n");
    }
}
