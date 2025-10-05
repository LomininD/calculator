#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "processor_properties.h"
#include "processor_cmd.h"
#include "stack/stack.h"
#include "stack/font_styles.h"
#include "stack/stack_dump.h"

// make printf macros for debug
// stack information and program info in debug file if mode is not debug

err_t execute_cmd(st_t* st, int* code, size_t* ip, proc_commands cmd);
err_t initialise_stack(size_t capacity, st_t* st);


//TODO: split output streams ???
//TODO: normal push input
// refactor stack only in case of splitting streams

int main()
{
    printf(MAKE_BOLD("+++ LMD PROCESSOR +++\n"));

    size_t capacity = 0;
    st_t st = {};
    err_t initialised = initialise_stack(capacity, &st);

    if (initialised != ok)
    {
        printf("main: terminating process due to error\n");
        return 0;
    }

    FILE* fp = fopen("program.out", "r"); // add ability to determine file

    int code[50] = {}; // refactor
    int cmd = 0;
    int i = 0;
    size_t prg_size = 0;
    int scanned = 0;

    while (true) // add read checking
    {
        scanned = fscanf(fp, "%d", &cmd);

        if (scanned == -1)
            break;

        //printf("scanned_cmd: %d\n", cmd);
        prg_size++;
        code[i] = cmd;
        i++;
    }
    //printf("prg_size: %zu\n", prg_size);

    size_t ip = 0;
    proc_commands current_cmd = UNKNOWN;

    while (ip < prg_size)
    {
        current_cmd = (proc_commands) code[ip];

        // printf("current_cmd = %d\n", current_cmd);

        err_t executed = execute_cmd(&st, code, &ip, current_cmd);

        if (executed != ok)
        {
            printf("main: terminating process due to error\n");
            return 0;
        }

        if (current_cmd == HLT)
        {
            fclose(fp);
            printf("main: shutting down processor\n");
            return 0;
        }

        getchar(); // optionally
        ip++;
    }

    printf("main: forced process termination, no HLT got\n");
    fclose(fp);
    return 0;
}


err_t execute_cmd(st_t* st, int* code, size_t* ip, proc_commands cmd)
{
    assert(st != NULL);

    err_t executed = ok;

    switch (cmd)
    {
        case PUSH:
            executed = proc_push(st, code, ip);
            break;

        case ADD:
            executed = proc_calc(st, ADD);
            break;

        case SUB:
            executed = proc_calc(st, SUB);
            break;

        case MULT:
            executed = proc_calc(st, MULT);
            break;

        case DIV:
            executed = proc_calc(st, DIV);
            break;

        case SQRT:
            executed = proc_calc(st, SQRT);
            break;

        case OUT:
            executed = proc_out(st);
            break;

        case HLT:
            executed = proc_hlt(st);
            break;

        default:
            printf("execute_cmd: unknown command (cmd = %d, ip = %zu), cannot execute\n", cmd, *ip);
    }

    if (executed == ok)
    {
        return ok;
    }
    else
    {
        return error;
    }
}


err_t initialise_stack(size_t capacity, st_t* st)
{
    assert(st != NULL);

    st_return_err created = st_ctor(st, capacity);

    if (created == no_error)
    {
        printf("initialise_stack: stack created\n");
        return ok;
    }
    else
    {
        printf("initialise_stack: problem occurred while creating stack\n");
        return stack_error;
    }
}


