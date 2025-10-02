#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack/stack.h"
#include "stack/font_styles.h"
#include "calculator_cmd.h"
#include "stack/stack_dump.h"

calc_commands determine_cmd(char* cmd);
void execute_cmd(st_t* st, calc_commands cmd);
st_t initialise_stack(size_t capacity);

//TODO: split output streams
//TODO: normal push input

int main()
{
    printf(MAKE_BOLD("+++ CALCULATOR +++\n"));

    size_t capacity = 10;
    st_t st = initialise_stack(capacity);

    char raw_cmd[10] = ""; // bad solution
    int scanned = scanf("%s", raw_cmd);

    while (scanned != -1)
    {
        clear_buffer();

        calc_commands cmd = determine_cmd(raw_cmd);

        execute_cmd(&st, cmd);

        if (cmd == HLT)
        {
            printf("main: shutting down calculator\n");
            return 0;
        }

        scanned = scanf("%s", raw_cmd);
    }

    printf("process terminated manually, no HLT got\n");
    return 0;
}


calc_commands determine_cmd(char* cmd)
{
    assert(cmd != NULL);

    if (strcmp("PUSH", cmd) == 0)
    {
        printf("determine_cmd: push recognised\n");
        return PUSH;
    }
    else if (strcmp("ADD", cmd) == 0)
    {
        printf("determine_cmd: add recognised\n");
        return ADD;
    }
    else if (strcmp("SUB", cmd) == 0)
    {
        printf("determine_cmd: sub recognised\n");
        return SUB;
    }
    else if (strcmp("DIV", cmd) == 0)
    {
        printf("determine_cmd: div recognised\n");
        return DIV;
    }
    else if (strcmp("MULT", cmd) == 0)
    {
        printf("determine_cmd: mult recognised\n");
        return MULT;
    }
    else if (strcmp("OUT", cmd) == 0)
    {
        printf("determine_cmd: out recognised\n");
        return OUT;
    }
    else if (strcmp("HLT", cmd) == 0)
    {
        printf("determine_cmd: hlt recognised\n");
        return HLT;
    }

    printf("determine_cmd: unknown command\n");
    return UNKNOWN;
}


void execute_cmd(st_t* st, calc_commands cmd)
{
    assert(st != NULL);

    switch (cmd)
    {
        case PUSH:
            calc_push(st);
            break;

        case ADD:
            calc_add(st);
            break;

        case SUB:
            calc_sub(st);
            break;

        case MULT:
            calc_mult(st);
            break;

        case DIV:
            calc_div(st);
            break;

        case OUT:
            calc_out(st);
            break;

        case HLT:
            calc_hlt(st);
            break;

        default:
            printf("execute_cmd: unknown command, cannot execute\n");
    }
}


st_t initialise_stack(size_t capacity)
{
    st_t st = {};
    st_return_err created = st_ctor(&st, capacity);

    if (created == no_error)
        printf("initialise_stack: stack created\n");
    else
        printf("initialise_stack: problem occurred while creating stack\n");

    return st;
}





