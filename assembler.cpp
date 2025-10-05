#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack/font_styles.h"
#include "processor_properties.h"

// solve problem with unexpected symbols
// add output to program.out
// add file paths
// debug messages in console (in debug mode) - user can redirect them to file
// debug messages to log file (always)
// errors and introductory information in console (always)


err_t determine_cmd(const char* file_name, int current_line, char* raw_cmd, proc_commands* cmd, char* str);
err_t read_arg(const char* file_name, int current_line, int* number, char* str);
char* readline(char* str, size_t* len, int* end);

int main()
{
    printf(MAKE_BOLD("+++ ASSEMBLER +++\n\n"));
    printf("version: %d\n", version);
    printf("designed by LMD (c) \n\n");

    printf("assembler: began assembly\n\n");

    int current_line = 0;
    const char* file_name = "program.asm";

    size_t len = 50;
    char* str = (char*) calloc(len + 1, sizeof(char));
    char raw_cmd[32] = {};
    proc_commands cmd = UNKNOWN;
    int end = 0;

    int got_hlt = 0;
    int got_out = 0;
    int got_something = 0;

    while (!end)
    {
        current_line++;
        str = readline(str, &len, &end);

        if (end)
            break;

        got_something = 1;
        printf("assembler: got str: %s\n", str);
        int scanned = sscanf(str, "%31s", raw_cmd);

        if (scanned == -1)
        {
            printf("\n");
            continue;
        }

        printf("assembler: got instruction: %s\n", raw_cmd);
        err_t recognized = determine_cmd(file_name, current_line, raw_cmd, &cmd, str);

        if (recognized != ok)
        {
            printf("\n");
            printf("assembler: " MAKE_BOLD_RED("aborting due to error\n"));
            return 0;
        }

        printf("\n");

        if (cmd == HLT)
        {
            got_hlt = 1;
            break;
        }

        if (cmd == OUT)
            got_out = 1;
        else
            got_out = 0;
    }

    free(str);

    if (got_something)
    {
        if (!got_out)
        {
            printf("assembler: %s:%d: " MAKE_BOLD("WARNING:") " calculations seem to have no effect\n", file_name, current_line);
            printf(MAKE_GREY("Note: you may forgot OUT instruction in your code\n\n"));
        }

        if (!got_hlt)
            printf("assembler: %s:%d: " MAKE_BOLD("WARNING:") " no HLT instruction in the end of the program\n\n", file_name, current_line);
    }
    else
    {
        printf("assembler: " MAKE_BOLD("WARNING:") " got blank file\n\n");
    }

    printf("assembler: finished assembly\n");
    return 0;
}


err_t read_arg(const char* file_name, int current_line, int* number, char* str)
{
    assert(number != NULL);
    assert(str != NULL);

    int success = sscanf(str, "%*s %d", number);

    if (success == 1)
    {
        printf("read_arg: recognised arg %d\n", *number);
        printf("%d\n", *number);
        return ok;
    }
    else
    {
        printf("\n");
        printf("read_arg: %s:%d: " MAKE_BOLD_RED("ERROR:") " failed to get arg\n", file_name, current_line);
        return wrong_number;
    }
}


err_t determine_cmd(const char* file_name, int current_line, char* raw_cmd, proc_commands* cmd, char* str)
{
    assert(raw_cmd != NULL);
    assert(cmd != NULL);
    assert(str != NULL);

    if (strcmp("PUSH", raw_cmd) == 0)
    {
        printf("determine_cmd: recognized push\n");

        int number = 0;
        err_t is_read = read_arg(file_name, current_line, &number, str);

        if (is_read != ok)
            return error;

        printf("%d\n", PUSH);
        *cmd = PUSH;

        return ok;
    }
    else if (strcmp("ADD", raw_cmd) == 0)
    {
        printf("determine_cmd: recognized add\n");
        printf("%d\n", ADD);
        *cmd = ADD;

        return ok;
    }
    else if (strcmp("SUB", raw_cmd) == 0)
    {
        printf("determine_cmd: recognized sub\n");
        printf("%d\n", SUB);
        *cmd = SUB;
        return ok;
    }
    else if (strcmp("DIV", raw_cmd) == 0)
    {
        printf("determine_cmd: recognized div\n");
        printf("%d\n", DIV);
        *cmd = DIV;
        return ok;
    }
    else if (strcmp("MULT", raw_cmd) == 0)
    {
        printf("determine_cmd: recognized mult\n");
        printf("%d\n", MULT);
        *cmd = MULT;
        return ok;
    }
    else if (strcmp("SQRT", raw_cmd) == 0)
    {
        printf("determine_cmd: recognized sqrt\n");
        printf("%d\n", SQRT);
        *cmd = SQRT;
        return ok;
    }
    else if (strcmp("OUT", raw_cmd) == 0)
    {
        printf("determine_cmd: recognized out\n");
        printf("%d\n", OUT);
        *cmd = OUT;
        return ok;
    }
    else if (strcmp("HLT", raw_cmd) == 0)
    {
        printf("determine_cmd: recognized hlt\n");
        printf("%d\n", HLT);
        *cmd = HLT;
        return ok;
    }

    printf("\n");
    printf("determine_cmd: %s:%d: " MAKE_BOLD_RED("ERROR:") " invalid command (%s)\n", file_name, current_line, raw_cmd);
    *cmd = UNKNOWN;
    return error;
}


char* readline(char* str, size_t* len, int* end)
{
    size_t symbol_ind = 0;
    char c = 0;

    while (true)
    {
        c = getchar();

        if (c == '\n' || c == '\r' || c == EOF)
        {
            if (c == EOF)
                *end = 1;
            break;
        }

        if (c == ';')
            *(str + symbol_ind) = '\0';
        else
            *(str + symbol_ind) = toupper(c);

        symbol_ind++;

        if (symbol_ind == *len)
        {
            *len *= 2;
            str = (char*) realloc(str, (*len + 1) * sizeof(char));
        }
    }

    *(str + symbol_ind) = '\0';
    return str;
}
