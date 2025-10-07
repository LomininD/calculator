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

struct proc_info
{
    int code[100]; // refactor using calloc and realloc or store file size in file
    size_t prg_size;
    st_t st;
    int registers[8];
    size_t ip;
};

// make printf macros for debug
// stack information and program info in debug file if mode is not debug

err_t proc_ctor(FILE* fp, proc_info* proc);
err_t prepare_file(FILE* fp, proc_info* proc);
err_t execute_cmd(st_t* st, int* code, size_t* ip, proc_commands cmd);
err_t initialise_stack(size_t capacity, st_t* st);

// if function couldn't complete the direct task it prints error message and returns error
// if function gets error message from other function it redirects it to the
// function from which was called
//TODO: split output streams ???
// refactor stack only in case of splitting streams

int main()
{
    printf(MAKE_BOLD("+++ PROCESSOR +++\n"));

    FILE* fp = fopen("program.out", "r"); // add ability to determine file

    proc_info proc = {};

    err_t initialised = proc_ctor(fp, &proc);

    if (initialised != ok)
    {
        printf("main: terminating process due to error\n");
        return 0;
    }

    int cmd = 0;
    int i = 0;
    int scanned = 0;

    while (true) // add read checking (corrupted byte code) and add new fuction
    {
        scanned = fscanf(fp, "%d", &cmd);

        if (scanned == -1)
            break;

        //printf("scanned_cmd: %d\n", cmd);
        proc.prg_size++;
        proc.code[i] = cmd;
        i++;
    }
    //printf("prg_size: %zu\n", prg_size);

    size_t ip = 0;
    proc_commands current_cmd = UNKNOWN;

    while (ip < proc.prg_size)
    {
        current_cmd = (proc_commands) proc.code[ip];

        // printf("current_cmd = %d\n", current_cmd);

        err_t executed = execute_cmd(&proc.st, proc.code, &ip, current_cmd);

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


err_t proc_ctor(FILE* fp, proc_info* proc)
{
    assert(proc != NULL);

    printf("proc_ctor: began initialising processor\n");

    // initialise stack, structure, check version, signature and get prog_size
    size_t capacity = 10;
    err_t initialised = initialise_stack(capacity, &(proc->st));

    if (initialised != ok)
        return error;

    proc->ip = 0;
    proc->prg_size = 0;

    err_t prepared = prepare_file(fp, proc);
    if (prepared != ok)
        return error;

    printf("proc_ctor: processor initialised\n\n");
    return ok;
}


err_t prepare_file(FILE* fp, proc_info* proc)
{
    assert(fp != NULL);
    assert(proc != NULL);

    int c = 0;
    int correct_symbols = 0;

    fscanf(fp, "%d", &c);
    if (c == 'L')
        correct_symbols++;
    fscanf(fp, "%d", &c);
    if (c == 'M')
        correct_symbols++;
    fscanf(fp, "%d", &c);
    if (c == 'D')
        correct_symbols++;

    if (correct_symbols == 3)
    {
        printf("prepare_file: file signature verified\n");
    }
    else
    {
        printf("prepare_file: " MAKE_BOLD_RED("ERROR:") " wrong file signature\n");
        return error;
    }

    int file_version = 0;
    fscanf(fp, "%d", &file_version);
    if (file_version == version)
    {
        printf("prepare_file: assembly version verified\n");
    }
    else
    {
        printf("prepare_file: " MAKE_BOLD_RED("ERROR:") " assembly version is old\n");
        return error;
    }

    return ok;
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


