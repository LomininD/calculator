#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack/font_styles.h"
#include "processor_properties.h"


struct files_info
{
    char* input_file_name;
    char* output_file_name;
    FILE* input_file;
    FILE* output_file;
};

struct assembler_info
{
    size_t len;
    char* str;
    char raw_cmd[32];
    proc_commands cmd;
    int end;
};

struct debug_info
{
    int current_line;
    int got_hlt;
    int got_out;
    int got_something;
};


// if no argument for output is given, output to file with the same name but .out extention
// solve problem with unexpected symbols
// debug messages in console (in debug mode) - user can redirect them to file
// debug messages to log file (always)
// errors and introductory information in console (always)

err_t initialise_assembler(int argc, char* argv[], files_info* files, assembler_info* asm_data, debug_info* debug);
err_t fill_file_preamble(files_info* files);
err_t determine_cmd(files_info* files, assembler_info* asm_data, debug_info* debug);
err_t read_number_arg(files_info* files, assembler_info* asm_data, debug_info* debug, int* number);
void readline(assembler_info* asm_data, files_info* file);
void check_warnings(debug_info* debug, files_info* files);


int main(int argc, char* argv[])
{
    printf(MAKE_BOLD("+++ ASSEMBLER +++\n\n"));
    printf("version: %d\n", version);
    printf("designed by LMD (c) \n\n");

    files_info files = {};
    assembler_info asm_data = {};
    debug_info debug = {};

    err_t initialised = initialise_assembler(argc, argv, &files, &asm_data, &debug);
    if (initialised != ok)
    {
        printf("\n");
        printf("assembler: " MAKE_BOLD_RED("aborting due to error\n"));
        return 0;
    }

    printf("assembler: began assembly\n\n");

    fill_file_preamble(&files);

    while (!asm_data.end)
    {
        debug.current_line++;
        readline(&asm_data, &files);

        if (asm_data.end)
            break;

        debug.got_something = 1;
        printf("assembler: got str: %s\n", asm_data.str);

        int scanned = sscanf(asm_data.str, "%31s", asm_data.raw_cmd);
        if (scanned == -1)
        {
            printf("\n");
            continue;
        }

        printf("assembler: got instruction: %s\n", asm_data.raw_cmd);

        err_t recognized = determine_cmd(&files, &asm_data, &debug);
        if (recognized != ok)
        {
            printf("\n");
            printf("assembler: " MAKE_BOLD_RED("aborting due to error\n"));
            return 0;
        }

        printf("\n");

        if (asm_data.cmd == HLT)
        {
            debug.got_hlt = 1;
            break;
        }

        if (asm_data.cmd == OUT)
            debug.got_out = 1;
    }

    check_warnings(&debug, &files);

    free(asm_data.str);
    printf("assembler: finished assembly\n");
    fclose(files.input_file);
    fclose(files.output_file);
    return 0;
}


err_t initialise_assembler(int argc, char* argv[], files_info* files, assembler_info* asm_data, debug_info* debug)
{
    assert(argv != NULL);
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    printf("initialise_assembler: began initialising\n");

    files->output_file_name = (char*) "program.out";

    asm_data->len = 50;
    asm_data->str = (char*) calloc(asm_data->len + 1, sizeof(char));
    asm_data->cmd = UNKNOWN;
    asm_data->end = 0;

    debug->current_line = 0;
    debug->got_hlt = 0;
    debug->got_out = 0;
    debug->got_something = 0;

    if (argc < 2)
    {
        printf("initialise_assembler: " MAKE_BOLD_RED("ERROR:") " required files were not provided\n");
        return error;

    }
    else if (argc > 3)
    {
        printf("initialise_assembler: " MAKE_BOLD_RED("ERROR:") " got to many files\n");
        return error;
    }
    else
    {
        files->input_file_name = argv[1];
        files->input_file = fopen(files->input_file_name, "r");

        if (argc == 3)
            files->output_file_name = argv[2];

        files->output_file = fopen(files->output_file_name, "w");

        if (files->input_file == NULL || files->output_file == NULL)
        {
            printf("initialise_assembler: " MAKE_BOLD_RED("ERROR:") " a problem with opening files occurred\n");
            printf(MAKE_GREY("Note: file name is likely to be incorrect\n"));
            return error;
        }

        printf("initialise_assembler: finished initialising\n");
        return ok;
    }
}


err_t fill_file_preamble(files_info* files)
{
    assert(files != NULL);

    fprintf(files->output_file, "%d\n", 'L');
    fprintf(files->output_file, "%d\n", 'M');
    fprintf(files->output_file, "%d\n", 'D');

    fprintf(files->output_file, "%d\n", version);

    return ok;
}


err_t read_number_arg(files_info* files, assembler_info* asm_data, debug_info* debug, int* number)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);
    assert(number != NULL);

    int success = sscanf(asm_data->str, "%*s %d", number);

    if (success == 1)
    {
        printf("read_number_arg: recognised arg %d\n", *number);
        fprintf(files->output_file, "%d\n", *number);
        return ok;
    }
    else
    {
        printf("\n");
        printf("read_number_arg: %s:%d: " MAKE_BOLD_RED("ERROR:") " failed to get arg\n", files->input_file_name, debug->current_line);
        return wrong_number;
    }
}


err_t determine_cmd(files_info* files, assembler_info* asm_data, debug_info* debug)
{
    assert(files != NULL);
    assert(asm_data != NULL);
    assert(debug != NULL);

    if (strcmp("PUSH", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized push\n");

        fprintf(files->output_file, "%d\n", PUSH);
        asm_data->cmd = PUSH;

        int number = 0;
        err_t is_read = read_number_arg(files, asm_data, debug, &number);

        if (is_read != ok)
            return error;

        return ok;
    }
    else if (strcmp("ADD", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized add\n");
        fprintf(files->output_file, "%d\n", ADD);
        asm_data->cmd = ADD;

        return ok;
    }
    else if (strcmp("SUB", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized sub\n");
        fprintf(files->output_file, "%d\n", SUB);
        asm_data->cmd = SUB;
        return ok;
    }
    else if (strcmp("DIV", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized div\n");
        fprintf(files->output_file, "%d\n", DIV);
        asm_data->cmd = DIV;
        return ok;
    }
    else if (strcmp("MULT", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized mult\n");
        fprintf(files->output_file, "%d\n", MULT);
        asm_data->cmd = MULT;
        return ok;
    }
    else if (strcmp("SQRT", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized sqrt\n");
        fprintf(files->output_file, "%d\n", SQRT);
        asm_data->cmd = SQRT;
        return ok;
    }
    else if (strcmp("OUT", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized out\n");
        fprintf(files->output_file, "%d\n", OUT);
        asm_data->cmd = OUT;
        return ok;
    }
    else if (strcmp("JMP", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized jmp\n");
        fprintf(files->output_file, "%d\n", JMP);
        asm_data->cmd = JMP;

        int number = 0;
        err_t is_read = read_number_arg(files, asm_data, debug, &number);

        if (is_read != ok)
            return error;

        return ok;
    }
    else if (strcmp("JB", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized jb\n");
        fprintf(files->output_file, "%d\n", JB);
        asm_data->cmd = JB;

        int number = 0;
        err_t is_read = read_number_arg(files, asm_data, debug, &number);

        if (is_read != ok)
            return error;

        return ok;
    }
    else if (strcmp("JBE", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized jbe\n");
        fprintf(files->output_file, "%d\n", JBE);
        asm_data->cmd = JBE;

        int number = 0;
        err_t is_read = read_number_arg(files, asm_data, debug, &number);

        if (is_read != ok)
            return error;

        return ok;
    }
    else if (strcmp("JA", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized ja\n");
        fprintf(files->output_file, "%d\n", JA);
        asm_data->cmd = JA;

        int number = 0;
        err_t is_read = read_number_arg(files, asm_data, debug, &number);

        if (is_read != ok)
            return error;

        return ok;
    }
    else if (strcmp("JAE", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized jae\n");
        fprintf(files->output_file, "%d\n", JAE);
        asm_data->cmd = JAE;

        int number = 0;
        err_t is_read = read_number_arg(files, asm_data, debug, &number);

        if (is_read != ok)
            return error;

        return ok;
    }
    else if (strcmp("JE", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized je\n");
        fprintf(files->output_file, "%d\n", JE);
        asm_data->cmd = JE;

        int number = 0;
        err_t is_read = read_number_arg(files, asm_data, debug, &number);

        if (is_read != ok)
            return error;

        return ok;
    }
    else if (strcmp("JNE", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized jne\n");
        fprintf(files->output_file, "%d\n", JNE);
        asm_data->cmd = JNE;

        int number = 0;
        err_t is_read = read_number_arg(files, asm_data, debug, &number);
        if (is_read != ok)
            return error;

        return ok;
    }
    else if (strcmp("HLT", asm_data->raw_cmd) == 0)
    {
        printf("determine_cmd: recognized hlt\n");
        fprintf(files->output_file, "%d\n", HLT);
        asm_data->cmd = HLT;
        return ok;
    }

    printf("\n");
    printf("determine_cmd: %s:%d: " MAKE_BOLD_RED("ERROR:") " invalid command (%s)\n", files->input_file_name, debug->current_line, asm_data->raw_cmd);
    asm_data->cmd = UNKNOWN;
    return error;
}


void readline(assembler_info* asm_data, files_info* file)
{
    assert(asm_data != NULL);
    assert(file != NULL);

    size_t symbol_ind = 0;
    char c = 0;

    while (true)
    {
        c = fgetc(file->input_file);

        if (c == '\n' || c == '\r' || c == EOF)
        {
            if (c == EOF)
                asm_data->end = 1;

            break;
        }

        if (c == ';')
            *(asm_data->str + symbol_ind) = '\0';
        else
            *(asm_data->str + symbol_ind) = toupper(c);

        symbol_ind++;

        if (symbol_ind == asm_data->len)
        {
            asm_data->len *= 2;
            asm_data->str = (char*) realloc(asm_data->str, (asm_data->len + 1) * sizeof(char));
        }
    }

    *(asm_data->str + symbol_ind) = '\0';
}


void check_warnings(debug_info* debug, files_info* files)
{
    assert(debug != NULL);
    assert(files != NULL);

    if (debug->got_something)
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
